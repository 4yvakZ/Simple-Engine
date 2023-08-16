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
		virtual void Construct() override;
		virtual void Init() override;
		virtual void Update() override;

		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};



		DirectX::SimpleMath::Matrix GetView() const;
		DirectX::SimpleMath::Matrix GetProjection() const;
		DirectX::SimpleMath::Matrix GetViewProjection() const;

		DirectX::SimpleMath::Vector3 GetForward();
		DirectX::SimpleMath::Vector3 GetRight();
		DirectX::SimpleMath::Vector3 GetUp();

		float GetFOVAngle() const;
		void SetFOVAngle(float fovAngle);

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
