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
	explicit MyGameObject(std:: string meshFileName = "../assets/sphere.fbx", std::shared_ptr<SimpleEngine::GameObject> parent = nullptr);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;

	void SetRotationSpeed(float speed);
private:
	//std::shared_ptr<SimpleEngine::SpriteComponent> mSpriteComponent;
	float mRotationSpeed = 0;

	std::shared_ptr<SimpleEngine::MeshComponent> mMeshComponent;
};

