#include "pch.h"
#include "Lamp.h"
#include "PointLightComponent.h"
#include "DebugRenderer.h"

Lamp::Lamp(std::shared_ptr<GameObject> parent):
	GameObject(parent)
{
	mPointLight = CreateComponent<SimpleEngine::PointLightComponent>();
	mPointLight->SetLightIntensity(DirectX::SimpleMath::Vector4::One * 2);
}

void Lamp::Update(float deltaTime)
{
	//SimpleEngine::DebugRenderer::GetInstance()->DrawDebugSphere(GetWorldTransform(), mPointLight->GetLightRadius());
}
