#include "pch.h"
#include "GBuffer.h"



SimpleEngine::GBuffer::GBuffer(DirectX::SimpleMath::Viewport viewport):
	mViewport(viewport)
{
}

void SimpleEngine::GBuffer::Init(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	InitDepthResouces(device);
	InitResource(device, mPositionTexture, mPositionSRV, mPositionRTV);
	InitResource(device, mNormalTexture, mNormalSRV, mNormalRTV);
	InitResource(device, mAlbedoTexture, mAlbedoSRV, mAlbedoRTV);
	InitResource(device, mMetallicRoughtnessAOTexture, mMetallicRoughtnessAOSRV, mMetallicRoughtnessAORTV);
	InitResource(device, mLightingTexture, mLightingSRV, mLightingRTV);
}

void SimpleEngine::GBuffer::Clear(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	using namespace DirectX::SimpleMath;
	context->ClearRenderTargetView(mPositionRTV.Get(), Color(0, 0, 0, 0));
	context->ClearRenderTargetView(mNormalRTV.Get(), Color(0, 0, 0, 0));
	context->ClearRenderTargetView(mAlbedoRTV.Get(), Color(0, 0, 0, 0));
	context->ClearRenderTargetView(mMetallicRoughtnessAORTV.Get(), Color(0, 0, 0, 0));
	context->ClearRenderTargetView(mLightingRTV.Get(), Color(0, 0, 0, 0));
	context->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SimpleEngine::GBuffer::Unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	ID3D11ShaderResourceView* nullsrvs[] = { nullptr, nullptr, nullptr, nullptr };
	context->PSSetShaderResources(0, 4, nullsrvs);
}

void SimpleEngine::GBuffer::SetAsTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetViewports(1, mViewport.Get11());

	ID3D11RenderTargetView* rtvs[] = { mPositionRTV.Get(), mAlbedoRTV.Get(), mNormalRTV.Get(), mMetallicRoughtnessAORTV.Get()};
	context->OMSetRenderTargets(4, rtvs, mDepthView.Get());
}

void SimpleEngine::GBuffer::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	ID3D11ShaderResourceView* srvs[] = { mPositionSRV.Get(), mNormalSRV.Get(), mAlbedoSRV.Get(), mMetallicRoughtnessAOSRV.Get() };
	context->PSSetShaderResources(0, 4, srvs);
}

void SimpleEngine::GBuffer::UnbindLighting(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(4, 1, nullsrv);
}

void SimpleEngine::GBuffer::SetLightingTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetViewports(1, mViewport.Get11());

	context->OMSetRenderTargets(1, mLightingRTV.GetAddressOf(), nullptr);
}

void SimpleEngine::GBuffer::BindLighting(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{

	context->PSSetShaderResources(4, 1, mLightingSRV.GetAddressOf());
}

void SimpleEngine::GBuffer::InitDepthResouces(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
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

	auto res = device->CreateTexture2D(&depthTexDesc, nullptr, mDepthBuffer.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	res = device->CreateShaderResourceView(mDepthBuffer.Get(), &shaderResourceViewDesc, mDepthSRV.GetAddressOf());
	res = device->CreateDepthStencilView(mDepthBuffer.Get(), &depthStencilViewDesc, mDepthView.GetAddressOf());
}

void SimpleEngine::GBuffer::InitResource(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<ID3D11Texture2D>& texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourseView, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& renderTargetView)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Height = mViewport.height;
	textureDesc.Width = mViewport.width;
	textureDesc.SampleDesc = { 1,0 };

	auto res = device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	res = device->CreateRenderTargetView(texture.Get(), &rtvDesc, renderTargetView.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	res = device->CreateShaderResourceView(texture.Get(), &srvDesc, shaderResourseView.GetAddressOf());
}
