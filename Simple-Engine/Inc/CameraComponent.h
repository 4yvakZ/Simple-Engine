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
		virtual void OnConstructed() override;
		virtual void Init() override;
		virtual void Update() override;

		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};



		DirectX::SimpleMath::Matrix GetView() const;
		DirectX::SimpleMath::Matrix GetProjection() const;
		DirectX::SimpleMath::Matrix CreateProjection(float nearPlane, float farPlane) const;
		DirectX::SimpleMath::Matrix GetViewProjection() const;

		DirectX::SimpleMath::Vector3 GetForward();
		DirectX::SimpleMath::Vector3 GetRight();
		DirectX::SimpleMath::Vector3 GetUp();

		float GetFarPlane() const;
		float GetNearPlane() const;

		float GetFOVAngle() const;
		void SetFOVAngle(float fovAngle);

	private:

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
