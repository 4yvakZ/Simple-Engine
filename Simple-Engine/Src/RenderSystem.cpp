#include "pch.h"
#include "RenderSystem.h"
#include "RenderComponent.h"

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

constexpr Color backgroundColor(0.2f, 0.2f, 0.2f);
//constexpr Color backgroundColor(0.f, 0.f, 0.f);

SimpleEngine::RenderSystem::RenderSystem(HWND hWnd, int clientWidth, int clientHeight)
{
	init(hWnd, clientWidth, clientHeight);
}

SimpleEngine::RenderSystem::~RenderSystem()
{
}

void SimpleEngine::RenderSystem::init(HWND hWnd, int clientWidth, int clientHeight)
{
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
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

	mViewport = Viewport(0.0f, 0.0f, static_cast<float>(clientWidth), static_cast<float>(clientHeight));

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
}

void SimpleEngine::RenderSystem::prepareFrame()
{
	mContext->ClearRenderTargetView(mRenderTarget.Get(), backgroundColor);
	mContext->OMSetRenderTargets(1, mRenderTarget.GetAddressOf(), nullptr);
	mContext->RSSetViewports(1, mViewport.Get11());
}

void SimpleEngine::RenderSystem::draw()
{
	for (auto it = mRenderComponents.begin(); it < mRenderComponents.end(); )
	{
		auto renderer = it->lock();

		if (renderer) 
		{
			renderer->draw(mContext);
			it++;
		} 
		else
		{
			it = mRenderComponents.erase(it);
		}
	}
}

void SimpleEngine::RenderSystem::endFrame()
{
	mContext->OMSetRenderTargets(0, nullptr, nullptr);

	mSwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void SimpleEngine::RenderSystem::addRenderComponent(std::shared_ptr<RenderComponent> renderComponent)
{
	mRenderComponents.emplace_back(renderComponent);
}
