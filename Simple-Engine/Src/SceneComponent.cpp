#include "pch.h"
#include "SceneComponent.h"
#include "GameObject.h"

using namespace SimpleEngine;

Transform SimpleEngine::SceneComponent::GetWorldTransform() const
{
    if (auto owner = GetOwner())
    {
        auto res = mTransform * GetOwner()->GetWorldTransform();
        return res;
    }
    return mTransform;
}
