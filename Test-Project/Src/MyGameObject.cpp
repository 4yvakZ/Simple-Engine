#include "pch.h"
#include "MyGameObject.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Material.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "Mesh.h"

using namespace SimpleEngine;

void MyGameObject::Init()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	//mSpriteComponent = CreateComponent<SpriteComponent>(material);

	auto meshes = AssetManager::ImportMeshes("../assets/sphere.fbx");

	mMeshComponent = CreateComponent<MeshComponent>(material, meshes[0]);

	GameObject::Init();
}
