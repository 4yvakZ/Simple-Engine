#pragma once
#include "GameObject.h"

namespace SimpleEngine
{
	class PointLightComponent;
}

class Lamp : public SimpleEngine::GameObject
{
public:
	Lamp(std::shared_ptr<GameObject> parent = nullptr);

	void Update(float deltaTime) override;

private:

	std::shared_ptr<SimpleEngine::PointLightComponent> mPointLight;
};

