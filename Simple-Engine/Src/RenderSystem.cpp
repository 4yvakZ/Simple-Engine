#include "pch.h"
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "DirectionalLightComponent.h"
#include "GBuffer.h"
#include "Material.h"
#include "AssetManager.h"
#include "DebugRenderer.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

constexpr Color kBackgroundColor(0.2f, 0.2f, 0.2f);
constexpr Color kDebugBackgroundColor(0.f, 0.f, 0.f);

SimpleEngine::RenderSystem::RenderSystem(HWND hWnd, int ClientWidth, int ClientHeight)
{
	Init(hWnd, ClientWidth, ClientHeight);
}

SimpleEngine::RenderSystem::~RenderSystem()
{
}

void SimpleEngine::RenderSystem::Init(HWND hWnd, int ClientWidth, int ClientHeight)
{
	mViewport = Viewport(0.0f, 0.0f, static_cast<float>(ClientWidth), static_cast<float>(ClientHeight));

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = ClientWidth;
	swapDesc.BufferDesc.Height = ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		//vAdapters[0],
		//D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		mSwapChain.GetAddressOf(),
		mDevice.GetAddressOf(),
		nullptr,
		mContext.GetAddressOf());

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), IID_PPV_ARGS_Helper(mBackBuffer.GetAddressOf()));	// __uuidof(ID3D11Texture2D)
	mDevice->CreateRenderTargetView(mBackBuffer.Get(), nullptr, mRenderTarget.GetAddressOf());


	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	InitFrameConstBuffer();

	InitBlendState();

	InitDepthStencilStateOff();

	mGBuffer = std::make_unique<GBuffer>(mViewport);
	mGBuffer->Init(mDevice);

	mDebugRenderer = DebugRenderer::GetInstance();
	mDebugRenderer->Init(mDevice);
}

void SimpleEngine::RenderSystem::InitDepthStencilStateOff()
{
	D3D11_DEPTH_STENCIL_DESC stateDesc = {};
	stateDesc.DepthEnable = false;

	mDevice->CreateDepthStencilState(&stateDesc, mDepthStencilStateOff.GetAddressOf());
}

void SimpleEngine::RenderSystem::InitBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
}

void SimpleEngine::RenderSystem::PrepareFrame()
{
	switch (mDebugFlag)
	{
	case SimpleEngine::RenderSystem::DebugFlag::None:
		mContext->ClearRenderTargetView(mRenderTarget.Get(), kBackgroundColor);
		break;
	default:
		mContext->ClearRenderTargetView(mRenderTarget.Get(), kDebugBackgroundColor);
		break;
	}

	mGBuffer->Clear(mContext);
}

void SimpleEngine::RenderSystem::Draw()
{
	mContext->VSSetConstantBuffers(0, 1, mFrameConstBuffer.GetAddressOf());
	mContext->PSSetConstantBuffers(0, 1, mFrameConstBuffer.GetAddressOf());
	
	mGBuffer->Unbind(mContext);

	ID3D11RenderTargetView* rtvs[] = { nullptr, nullptr, nullptr, nullptr};
	switch (mDebugFlag)
	{
	case RenderSystem::DebugFlag::WorldPosition:
		mContext->RSSetViewports(1, mViewport.Get11());
		rtvs[static_cast<int>(mDebugFlag)] = mRenderTarget.Get();
		mContext->OMSetRenderTargets(4, rtvs, mGBuffer->GetDepthStencilView().Get());
		break;

	case RenderSystem::DebugFlag::Normals:
		mContext->RSSetViewports(1, mViewport.Get11());
		rtvs[static_cast<int>(mDebugFlag)] = mRenderTarget.Get();
		mContext->OMSetRenderTargets(4, rtvs, mGBuffer->GetDepthStencilView().Get());
		break;

	case RenderSystem::DebugFlag::Albedo:
		mContext->RSSetViewports(1, mViewport.Get11());
		rtvs[static_cast<int>(mDebugFlag)] = mRenderTarget.Get();
		mContext->OMSetRenderTargets(4, rtvs, mGBuffer->GetDepthStencilView().Get());
		break;

	case RenderSystem::DebugFlag::MetallicRoughnessAO:
		mContext->RSSetViewports(1, mViewport.Get11());
		rtvs[static_cast<int>(mDebugFlag)] = mRenderTarget.Get();
		mContext->OMSetRenderTargets(4, rtvs, mGBuffer->GetDepthStencilView().Get());
		break;

	default:
		mGBuffer->SetAsTarget(mContext);
		break;
	}
	

	for (auto it = mRenderComponents.begin(); it < mRenderComponents.end(); )
	{
		auto renderer = it->lock();

		if (renderer)
		{
			renderer->Draw(mContext);
			it++;
		}
		else
		{
			it = mRenderComponents.erase(it);
		}
	}

	mGBuffer->UnbindLighting(mContext);

	switch (mDebugFlag)
	{
	case RenderSystem::DebugFlag::None:
		mGBuffer->SetLightingTarget(mContext);
		break;
	case RenderSystem::DebugFlag::Lighting:
		mContext->RSSetViewports(1, mViewport.Get11());
		mContext->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), mGBuffer->GetDepthStencilView().Get());
		break;
	default:
		break;
	}

	mContext->OMSetDepthStencilState(mDepthStencilStateOff.Get(), 0);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	mContext->OMSetBlendState(mBlendState.Get(), blendFactor, sampleMask);

	mGBuffer->Bind(mContext);

	//TODO RenderLight
	for (auto it = mDirectionalLightComponents.begin(); it < mDirectionalLightComponents.end(); )
	{
		auto dirLight = it->lock();

		if (dirLight)
		{
			dirLight->Draw(mContext);
			it++;
		}
		else
		{
			it = mDirectionalLightComponents.erase(it);
		}
	}

	mContext->OMSetBlendState(nullptr, blendFactor, sampleMask);

	switch (mDebugFlag)
	{
	case SimpleEngine::RenderSystem::DebugFlag::None:
		mContext->RSSetViewports(1, mViewport.Get11());
		mContext->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), mGBuffer->GetDepthStencilView().Get());
		mGBuffer->BindLighting(mContext);

		AssetManager::GetInstance()->GetDefaultColorPassMaterial()->Bind(mContext);
		mContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		mContext->Draw(4, 0);
		break;
	default:
		break;
	}
	
	mContext->OMSetDepthStencilState(nullptr, 0);

	switch (mDebugFlag)
	{
	case SimpleEngine::RenderSystem::DebugFlag::None:
		mDebugRenderer->Draw(mContext);
		break;
	default:
		break;
	}

	/*/
	mDirectionalLightComponents[0].lock()->Bind(mContext);

	for (auto it = mRenderComponents.begin(); it < mRenderComponents.end(); )
	{
		auto renderer = it->lock();

		if (renderer && renderer->GetMaterial()->IsDeferred())
		{
			renderer->Draw(mContext);
			it++;
		} 
		else
		{
			it = mRenderComponents.erase(it);
		}
	}*/
}

void SimpleEngine::RenderSystem::EndFrame()
{
	mContext->OMSetRenderTargets(0, nullptr, nullptr);

	mSwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void SimpleEngine::RenderSystem::Update(float deltaTime, const FrameConstBufferData& frameBufferData)
{
	mDebugRenderer->Update(deltaTime);

	mFrameConstBufferData = frameBufferData;
	UpdateFrameConstBuffer();
}

Microsoft::WRL::ComPtr<ID3D11Device> SimpleEngine::RenderSystem::getDevice() {
	return mDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> SimpleEngine::RenderSystem::getContext()
{
	return mContext;
}

void SimpleEngine::RenderSystem::AddRenderComponent(std::shared_ptr<RenderComponent> renderComponent)
{
	mRenderComponents.emplace_back(renderComponent);
}

void SimpleEngine::RenderSystem::AddDirectionalLightComponent(std::shared_ptr<DirectionalLightComponent> lightComponent)
{
	mDirectionalLightComponents.emplace_back(lightComponent);
}

void SimpleEngine::RenderSystem::SetDebugFlag(DebugFlag flag)
{
	if (mDebugFlag != flag) {
		mDebugFlag = flag;
	}
}

void SimpleEngine::RenderSystem::InitFrameConstBuffer()
{
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(mFrameConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &mFrameConstBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&constBufDesc, &constData, mFrameConstBuffer.GetAddressOf());
}

void SimpleEngine::RenderSystem::UpdateFrameConstBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	mContext->Map(mFrameConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mFrameConstBufferData, sizeof(FrameConstBufferData));

	mContext->Unmap(mFrameConstBuffer.Get(), 0);
}
