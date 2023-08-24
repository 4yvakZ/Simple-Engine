#pragma once
#include "GameObject.h"

namespace SimpleEngine
{
	class DirectionalLightComponent;
}

class Sun : public SimpleEngine::GameObject
{

	virtual void Init() override;
private:
	//std::shared_ptr<SimpleEngine::SpriteComponent> mSpriteComponent;

	std::shared_ptr<SimpleEngine::DirectionalLightComponent> mDirectionalLightComponent;
};

