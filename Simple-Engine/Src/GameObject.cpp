#include "pch.h"
#include "GameObject.h"
#include "Component.h"

void SimpleEngine::GameObject::update(float deltaTime)
{
	for (auto it = mComponents.begin(); it < mComponents.end(); it++)
	{
		(*it)->update();
	}
}

void SimpleEngine::GameObject::init()
{
	for (auto it = mComponents.begin(); it < mComponents.end(); it++)
	{
		(*it)->init();
	}
}

SimpleEngine::Transform SimpleEngine::GameObject::getWorldTransform() const
{
	if (auto parent = mParent.lock()) 
	{
		return mTransform * parent->getWorldTransform();
	}
	return mTransform;
}
