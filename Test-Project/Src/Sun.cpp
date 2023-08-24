#include "pch.h"
#include "Sun.h"
#include "Utils.h"

#include "DirectionalLightComponent.h"

Sun::Sun(std::shared_ptr<GameObject> parent):
	GameObject(parent)
{
	mDirectionalLightComponent = CreateComponent<SimpleEngine::DirectionalLightComponent>();
}

void Sun::Init()
{
	GameObject::Init();
}

void Sun::SetLightDirection(DirectX::SimpleMath::Vector3 direction)
{
	mDirectionalLightComponent->SetLightDirection(DirectX::SimpleMath::ToVector4(direction, 0.0));
}
