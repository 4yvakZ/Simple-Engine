#include "pch.h"
#include "CameraComponent.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

SimpleEngine::CameraComponent::CameraComponent() :
	mTarget(Vector3::Forward),
	mUp(Vector3::Up),
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
	auto pos = getWorldTransform().getPosition();
	auto target = pos + mTarget;
	mView = Matrix::CreateLookAt(pos, target, mUp);

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

void SimpleEngine::CameraComponent::init()
{
}
