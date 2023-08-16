#pragma once
#include "GameObject.h"

namespace SimpleEngine
{
	class SpriteComponent;
	class MeshComponent;
}

class MyGameObject : public SimpleEngine::GameObject
{
public:
	explicit MyGameObject(std::shared_ptr<SimpleEngine::GameObject> parent = nullptr) :
		SimpleEngine::GameObject(parent) {}

	virtual void init() override;
private:
	//std::shared_ptr<SimpleEngine::SpriteComponent> mSpriteComponent;

	std::shared_ptr<SimpleEngine::MeshComponent> mMeshComponent;
};

