#pragma once
#include "SceneComponent.h"
#include "ShaderStructs.h"

namespace SimpleEngine
{

	class CameraComponent : public SceneComponent
	{
	public:
		CameraComponent();

		// Inherited via SceneComponent
		virtual void construct() override;
		virtual void init() override;
		virtual void update() override;

		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		auto getViewProjection() const {
			return mView * mProjection;
		}

		auto getView() const {
			return mView;
		}

		auto getProjection() const {
			return mProjection;
		}


	private:
		ProjectionType mProjectionType = ProjectionType::Perspective;

		DirectX::SimpleMath::Matrix mView;
		DirectX::SimpleMath::Matrix mProjection;

		DirectX::SimpleMath::Vector3 mTarget;
		DirectX::SimpleMath::Vector3 mUp;

		float mFarPlane = 100.0f;
		float mNearPlane = 0.1f;
		float mFovAngle = DirectX::XM_PIDIV2;
		float mAspectRatio;

		float mOrtWidth;
		float mOrtHeight;
	};
}
