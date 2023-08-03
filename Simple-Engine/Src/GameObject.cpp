#include "pch.h"
#include "GameObject.h"
#include "Component.h"

void SimpleEngine::GameObject::update(float deltaTime)
{
}

void SimpleEngine::GameObject::init()
{
	for (auto it = mComponents.begin(); it < mComponents.end(); it++)
	{
		(*it)->init();
	}
}
