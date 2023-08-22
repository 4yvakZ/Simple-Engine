#include "pch.h"
#include "RenderSystem.h"
#include "RenderComponent.h"
#include "DirectionalLightComponent.h"
#include "GBuffer.h"
#include "Material.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

constexpr Color backgroundColor(0.2f, 0.2f, 0.2f);
//constexpr Color backgroundColor(0.f, 0.f, 0.f);

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

	D3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = mViewport.width;
	depthTexDesc.Height = mViewport.height;
	depthTexDesc.SampleDesc = { 1, 0 };

	res = mDevice->CreateTexture2D(&depthTexDesc, nullptr, &mDepthStencilBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	res = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf());

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	InitFrameConstBuffer();

	mGBuffer = std::make_unique<GBuffer>(mViewport);
	mGBuffer->Init(mDevice);
}

void SimpleEngine::RenderSystem::PrepareFrame()
{
	mContext->ClearRenderTargetView(mRenderTarget.Get(), backgroundColor);
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	mGBuffer->Clear(mContext);
}

void SimpleEngine::RenderSystem::Draw()
{
	mContext->VSSetConstantBuffers(0, 1, mFrameConstBuffer.GetAddressOf());
	mContext->PSSetConstantBuffers(0, 1, mFrameConstBuffer.GetAddressOf());
	mGBuffer->SetAsTarget(mContext);
	//mContext->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), mDepthStencilView.Get());

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

	mGBuffer->Bind(mContext);

	mContext->RSSetViewports(1, mViewport.Get11());
	mContext->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), mDepthStencilView.Get());
	

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

void SimpleEngine::RenderSystem::Update(const FrameConstBufferData& frameBufferData)
{
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
