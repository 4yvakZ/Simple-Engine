#include "pch.h"
#include "GameObject.h"
#include "Component.h"

void SimpleEngine::GameObject::Update(float deltaTime)
{
	for (auto it = mComponents.begin(); it < mComponents.end(); it++)
	{
		(*it)->Update();
	}
}

void SimpleEngine::GameObject::Init()
{
	for (auto it = mComponents.begin(); it < mComponents.end(); it++)
	{
		(*it)->Init();
	}
}

std::shared_ptr<SimpleEngine::GameObject> SimpleEngine::GameObject::GetParent()
{
	return mParent.lock();
}

void SimpleEngine::GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
	mParent = parent;
}

void SimpleEngine::GameObject::SetTransform(const Transform& transform)
{
	mTransform = transform;
}

const SimpleEngine::Transform& SimpleEngine::GameObject::GetTransform() const
{
	return mTransform;
}

SimpleEngine::Transform SimpleEngine::GameObject::GetWorldTransform() const
{
	if (auto parent = mParent.lock()) 
	{
		return mTransform * parent->GetWorldTransform();
	}
	return mTransform;
}

void SimpleEngine::GameObject::SetWorldTransform(const Transform& transform)
{
	if (auto parent = mParent.lock())
	{
		mTransform = transform / parent->GetWorldTransform();
	}
	mTransform = transform;
}
