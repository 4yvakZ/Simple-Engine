#include "pch.h"
#include "Sun.h"

#include "DirectionalLightComponent.h"

void Sun::Init()
{
	mDirectionalLightComponent = CreateComponent<SimpleEngine::DirectionalLightComponent>();

	GameObject::Init();
}
