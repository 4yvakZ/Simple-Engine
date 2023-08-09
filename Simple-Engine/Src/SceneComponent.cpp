#include "pch.h"
#include "SceneComponent.h"
#include "GameObject.h"

using namespace SimpleEngine;

Transform SimpleEngine::SceneComponent::getWorldTransform() const
{
    if (auto owner = getOwner())
    {
        auto res = mTransform * getOwner()->getWorldTransform();
        return res;
    }
    return mTransform;
}
