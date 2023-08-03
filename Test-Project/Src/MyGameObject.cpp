#include "pch.h"
#include "MyGameObject.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Material.h"
using namespace SimpleEngine;

void MyGameObject::init()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	mSpriteComponent = createComponent<SpriteComponent>(material);

	GameObject::init();
}
