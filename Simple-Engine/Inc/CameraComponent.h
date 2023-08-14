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



		DirectX::SimpleMath::Matrix getView() const;
		DirectX::SimpleMath::Matrix getProjection() const;
		DirectX::SimpleMath::Matrix getViewProjection() const;

		DirectX::SimpleMath::Vector3 getForward();
		DirectX::SimpleMath::Vector3 getRight();
		DirectX::SimpleMath::Vector3 getUp();

		float getFOVAngle() const;
		void setFOVAngle(float fovAngle);

	private:
		ProjectionType mProjectionType = ProjectionType::Perspective;

		DirectX::SimpleMath::Matrix mView;
		DirectX::SimpleMath::Matrix mProjection;

		float mFarPlane = 100.0f;
		float mNearPlane = 0.1f;
		float mFovAngle = DirectX::XM_PIDIV2;
		float mAspectRatio;

		float mOrtWidth;
		float mOrtHeight;
	};
}
