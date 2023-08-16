#pragma once
#include "Component.h"
#include "Transform.h"

namespace SimpleEngine
{
	class SceneComponent : public Component
	{
	public:
		void SetTransform(const Transform& transform)
		{
			mTransform = transform;
		}

		const Transform& GetTransform() const
		{
			return mTransform;
		}

		Transform GetWorldTransform() const;

	private:
		Transform mTransform;
	};
}

