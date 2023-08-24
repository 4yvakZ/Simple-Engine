#pragma once
#include "GameObject.h"

namespace SimpleEngine
{
	class DirectionalLightComponent;
}

class Sun : public SimpleEngine::GameObject
{
public:
	explicit Sun(std::shared_ptr<GameObject> parent = nullptr);

	virtual void Init() override;
	void SetLightDirection(DirectX::SimpleMath::Vector3 direction);

private:
	//std::shared_ptr<SimpleEngine::SpriteComponent> mSpriteComponent;

	std::shared_ptr<SimpleEngine::DirectionalLightComponent> mDirectionalLightComponent;
};

