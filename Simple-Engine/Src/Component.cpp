#include "pch.h"
#include "Component.h"

void SimpleEngine::Component::OnConstructed()
{
}

void SimpleEngine::Component::SetOwner(std::shared_ptr<GameObject> owner)
{
	mOwner = std::weak_ptr<GameObject>(owner);
}

std::shared_ptr<SimpleEngine::GameObject> SimpleEngine::Component::GetOwner() const
{
	return mOwner.lock();
}
