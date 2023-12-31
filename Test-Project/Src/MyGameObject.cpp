#include "pch.h"
#include "MyGameObject.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Material.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "DebugRenderer.h"

using namespace SimpleEngine;
using namespace DirectX::SimpleMath;

MyGameObject::MyGameObject(std::string meshFileName, std::shared_ptr<SimpleEngine::GameObject> parent) :
	SimpleEngine::GameObject(parent)
{
	auto meshes = AssetManager::GetInstance()->ImportMeshes(meshFileName);

	mMeshComponent = CreateComponent<MeshComponent>(meshes[0]);
}

void MyGameObject::Init()
{
	GameObject::Init();
}

void MyGameObject::Update(float deltaTime)
{
	if (mRotationSpeed != 0)
	{
		auto transform = GetTransform();
		transform.SetRotation(transform.GetRotation() * Quaternion::CreateFromAxisAngle(Vector3::Up, mRotationSpeed * deltaTime));
		SetTransform(transform);
		//DebugRenderer::GetInstance()->DrawDebugSphere(transform, 1);
	}	

	GameObject::Update(deltaTime);
}


void MyGameObject::SetRotationSpeed(float speed)
{
	mRotationSpeed = speed;
}
