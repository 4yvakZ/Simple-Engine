#include "pch.h"
#include "CameraComponent.h"
#include "Game.h"
#include "Utils.h"

using namespace DirectX::SimpleMath;

SimpleEngine::CameraComponent::CameraComponent() :
	mAspectRatio(),
	mOrtHeight(),
	mOrtWidth()
{
}

void SimpleEngine::CameraComponent::OnConstructed()
{
	auto game = Game::GetInstance();
	if (!game->GetActiveCameraComponent())
	{
		game->SetActiveCameraComp(std::dynamic_pointer_cast<CameraComponent>(shared_from_this()));
	}
}

void SimpleEngine::CameraComponent::Update()
{
	
	auto transform = GetWorldTransform();

	auto pos = transform.GetPosition();
	auto forward = GetForward();
	auto target = pos + forward;

	mView = Matrix::CreateLookAt(pos, target, GetUp());

	auto game = Game::GetInstance();
	int width = game->ClientWidth();
	int height = game->ClientHeight();

	mAspectRatio = static_cast<float>(width) / static_cast<float>(height);
	mOrtHeight = height * 0.02f;
	mOrtWidth = width * 0.02f;

	mProjection = CreateProjection(mNearPlane, mFarPlane);
}

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::CreateProjection(float nearPlane, float farPlane) const
{
	switch (mProjectionType)
	{
	case SimpleEngine::CameraComponent::ProjectionType::Perspective:
		return Matrix::CreatePerspectiveFieldOfView(
			mFovAngle,
			mAspectRatio,
			nearPlane,
			farPlane
		);
		break;
	case SimpleEngine::CameraComponent::ProjectionType::Orthographic:
		return Matrix::CreateOrthographic(
			mOrtWidth,
			mOrtHeight,
			nearPlane,
			farPlane
		);
		break;
	default:
		break;
	}

	
}

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::GetViewProjection() const {
	return mView * mProjection;
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::GetForward()
{
	auto transform = GetWorldTransform();
	return Vector3::TransformNormal(Vector3::Forward, transform.GetWorld());
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::GetRight()
{
	auto transform = GetWorldTransform();
	return Vector3::TransformNormal(Vector3::Right, transform.GetWorld());
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::GetUp()
{
	auto transform = GetWorldTransform();
	return Vector3::TransformNormal(Vector3::Up, transform.GetWorld());
}

float SimpleEngine::CameraComponent::GetFarPlane() const
{
	return mFarPlane;
}

float SimpleEngine::CameraComponent::GetNearPlane() const
{
	return mNearPlane;
}

float SimpleEngine::CameraComponent::GetFOVAngle() const
{
	return mFovAngle;
}

void SimpleEngine::CameraComponent::SetFOVAngle(float fovAngle)
{
	if (fovAngle > DirectX::XM_PI - DirectX::XM_PIDIV4)
	{
		mFovAngle = DirectX::XM_PI - DirectX::XM_PIDIV4;
		return;
	}

	if (fovAngle < DirectX::XM_PIDIV4)
	{
		mFovAngle = DirectX::XM_PIDIV4;
		return;
	}

	mFovAngle = fovAngle;
}

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::GetView() const {
	return mView;
}

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::GetProjection() const {
	return mProjection;
}

void SimpleEngine::CameraComponent::Init()
{
}
