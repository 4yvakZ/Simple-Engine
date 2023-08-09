#include "pch.h"
#include "Component.h"

void SimpleEngine::Component::construct()
{
}

void SimpleEngine::Component::setOwner(std::shared_ptr<GameObject> owner)
{
	mOwner = std::weak_ptr<GameObject>(owner);
}

std::shared_ptr<SimpleEngine::GameObject> SimpleEngine::Component::getOwner() const
{
	return mOwner.lock();
}
