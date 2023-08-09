#pragma once
#include "Component.h"
#include "Transform.h"

namespace SimpleEngine
{
	class SceneComponent : public Component
	{
	public:
		void setTransform(const Transform& transform)
		{
			mTransform = transform;
		}

		const Transform& getTransform() const
		{
			return mTransform;
		}

		Transform getWorldTransform() const;

	private:
		Transform mTransform;
	};
}

