#include "pch.h"
#include "MyGameObject.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Material.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "Mesh.h"

using namespace SimpleEngine;

void MyGameObject::init()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	//mSpriteComponent = createComponent<SpriteComponent>(material);

	auto meshes = AssetManager::importMeshes("../assets/sphere.fbx");

	mMeshComponent = createComponent<MeshComponent>(material, meshes[0]);

	GameObject::init();
}
