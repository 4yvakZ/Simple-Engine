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

void SimpleEngine::CameraComponent::Construct()
{
	auto game = Game::GetInstance();
	if (!game->GetActiveCameraComponent().lock())
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

	switch (mProjectionType)
	{
	case SimpleEngine::CameraComponent::ProjectionType::Perspective:
		mAspectRatio = static_cast<float>(width) / static_cast<float>(height);

		mProjection = Matrix::CreatePerspectiveFieldOfView(
			mFovAngle,
			mAspectRatio,
			mNearPlane,
			mFarPlane
		);
		break;
	case SimpleEngine::CameraComponent::ProjectionType::Orthographic:
		mOrtHeight = height * 0.02f;
		mOrtWidth = width * 0.02f;

		mProjection = Matrix::CreateOrthographic(
			mOrtWidth,
			mOrtHeight,
			mNearPlane,
			mFarPlane
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
