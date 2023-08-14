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

std::shared_ptr<SimpleEngine::GameObject> SimpleEngine::GameObject::getParent()
{
	return mParent.lock();
}

void SimpleEngine::GameObject::setParent(std::shared_ptr<GameObject> parent)
{
	mParent = parent;
}

void SimpleEngine::GameObject::setTransform(const Transform& transform)
{
	mTransform = transform;
}

const SimpleEngine::Transform& SimpleEngine::GameObject::getTransform() const
{
	return mTransform;
}

SimpleEngine::Transform SimpleEngine::GameObject::getWorldTransform() const
{
	if (auto parent = mParent.lock()) 
	{
		return mTransform * parent->getWorldTransform();
	}
	return mTransform;
}

void SimpleEngine::GameObject::setWorldTransform(const Transform& transform)
{
	if (auto parent = mParent.lock())
	{
		mTransform = transform / parent->getWorldTransform();
	}
	mTransform = transform;
}
