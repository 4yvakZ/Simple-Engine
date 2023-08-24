#include "pch.h"
#include "DirectionalLightComponent.h"

#include "Game.h"
#include "RenderSystem.h"
#include "Material.h"
#include "AssetManager.h"
#include "ShadowMap.h"

using namespace DirectX::SimpleMath;
constexpr Vector4 kDefaultLightDirection =  Vector4 (-1, -1, -1, 0);
constexpr Vector4 kDefaultLightIntensity = Vector4 (1, 1, 1, 0);
constexpr int kShadowMapWidth = 4096;

SimpleEngine::DirectionalLightComponent::DirectionalLightComponent():
	mMaterial(AssetManager::GetInstance()->GetDefaultLightMaterial())
{
	mLightConstBufferData.mDirection = kDefaultLightDirection;
	mLightConstBufferData.mDirection.Normalize();
	mLightConstBufferData.mIntensity = kDefaultLightIntensity;
}

void SimpleEngine::DirectionalLightComponent::Init()
{
	auto device = Game::GetRenderSystem()->getDevice();

	InitLightConstBuffer(device);
	
	mShadowMap->Init(device);
}

void SimpleEngine::DirectionalLightComponent::Update()
{
	UpdateLightConstBuffer();
}

void SimpleEngine::DirectionalLightComponent::OnConstructed()
{
	Component::OnConstructed();

	mShadowMap = std::make_unique<ShadowMap>(std::dynamic_pointer_cast<DirectionalLightComponent>(shared_from_this()), kShadowMapWidth);
	Game::GetRenderSystem()->AddDirectionalLightComponent(std::dynamic_pointer_cast<DirectionalLightComponent>(shared_from_this()));
}

void SimpleEngine::DirectionalLightComponent::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->PSSetConstantBuffers(2, 1, mLightConstBuffer.GetAddressOf());
}

void SimpleEngine::DirectionalLightComponent::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	mMaterial->Bind(context);
	Bind(context);
	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	context->Draw(4, 0);
}

void SimpleEngine::DirectionalLightComponent::BindShadowMap(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	mShadowMap->Bind(context);
}

void SimpleEngine::DirectionalLightComponent::SetShadowMapAsTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	mShadowMap->SetAsTarget(context);
}

void SimpleEngine::DirectionalLightComponent::ClearShadowMap(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	mShadowMap->Clear(context);
}

DirectX::SimpleMath::Vector4 SimpleEngine::DirectionalLightComponent::GetLightDirection() const
{
	return mLightConstBufferData.mDirection;
}

void SimpleEngine::DirectionalLightComponent::SetLightDirection(DirectX::SimpleMath::Vector4 lightDirection)
{
	mLightConstBufferData.mDirection = lightDirection;
}

DirectX::SimpleMath::Vector4 SimpleEngine::DirectionalLightComponent::GetLightIntensity() const
{
	return mLightConstBufferData.mIntensity;
}

void SimpleEngine::DirectionalLightComponent::SetLightIntensity(DirectX::SimpleMath::Vector4 lightIntensity)
{
	mLightConstBufferData.mIntensity = lightIntensity;
}

std::shared_ptr<SimpleEngine::Material> SimpleEngine::DirectionalLightComponent::GetMaterial() const
{
	return mMaterial;
}

void SimpleEngine::DirectionalLightComponent::SetMaterial(std::shared_ptr<Material> material)
{
	mMaterial = material;
}

void SimpleEngine::DirectionalLightComponent::InitLightConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(mLightConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &mLightConstBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;
	device->CreateBuffer(&constBufDesc, &constData, mLightConstBuffer.GetAddressOf());
}

void SimpleEngine::DirectionalLightComponent::UpdateLightConstBuffer()
{
	auto context = Game::GetRenderSystem()->getContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(mLightConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mLightConstBufferData, sizeof(FrameConstBufferData));

	context->Unmap(mLightConstBuffer.Get(), 0);
}
