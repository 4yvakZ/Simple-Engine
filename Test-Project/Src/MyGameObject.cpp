#include "pch.h"
#include "MyGameObject.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Material.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "Mesh.h"

using namespace SimpleEngine;
using namespace DirectX::SimpleMath;

void MyGameObject::Init()
{
	//mSpriteComponent = CreateComponent<SpriteComponent>(material);

	auto meshes = AssetManager::GetInstance()->ImportMeshes("../assets/sphere.fbx");

	mMeshComponent = CreateComponent<MeshComponent>(meshes[0]);

	GameObject::Init();
}

void MyGameObject::Update(float deltaTime)
{
	auto transform = GetTransform();
	transform.SetRotation(transform.GetRotation() * Quaternion::CreateFromAxisAngle(Vector3::Up, mRotationSpeed * deltaTime));
	SetTransform(transform);

	GameObject::Update(deltaTime);
}


void MyGameObject::SetRotationSpeed(float speed)
{
	mRotationSpeed = speed;
}
