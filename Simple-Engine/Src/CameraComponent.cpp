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

void SimpleEngine::CameraComponent::construct()
{
	auto game = Game::getInstance();
	if (!game->getActiveCameraComponent().lock())
	{
		game->setActiveCameraComp(std::dynamic_pointer_cast<CameraComponent>(shared_from_this()));
	}
}

void SimpleEngine::CameraComponent::update()
{
	
	auto transform = getWorldTransform();

	auto pos = transform.getPosition();
	auto forward = getForward();
	auto target = pos + forward;

	mView = Matrix::CreateLookAt(pos, target, getUp());

	auto game = Game::getInstance();
	int width = game->clientWidth();
	int height = game->clientHeight();

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

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::getViewProjection() const {
	return mView * mProjection;
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::getForward()
{
	auto transform = getWorldTransform();
	return Vector3::TransformNormal(Vector3::Forward, transform.getWorld());
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::getRight()
{
	auto transform = getWorldTransform();
	return Vector3::TransformNormal(Vector3::Right, transform.getWorld());
}

DirectX::SimpleMath::Vector3 SimpleEngine::CameraComponent::getUp()
{
	auto transform = getWorldTransform();
	return Vector3::TransformNormal(Vector3::Up, transform.getWorld());
}

float SimpleEngine::CameraComponent::getFOVAngle() const
{
	return mFovAngle;
}

void SimpleEngine::CameraComponent::setFOVAngle(float fovAngle)
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

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::getView() const {
	return mView;
}

DirectX::SimpleMath::Matrix SimpleEngine::CameraComponent::getProjection() const {
	return mProjection;
}

void SimpleEngine::CameraComponent::init()
{
}
