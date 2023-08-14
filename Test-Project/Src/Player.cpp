#include "pch.h"
#include "Player.h"
#include "CameraComponent.h"

using namespace SimpleEngine;

void Player::init()
{
	using namespace DirectX::SimpleMath;
	mCameraComponent = createComponent<CameraComponent>();
	auto camTransform = mCameraComponent->getTransform();
	camTransform.setPosition(Vector3(0, 0, 0));
	mCameraComponent->setTransform(camTransform);
	Game::getInputDevice()->MouseMove.AddRaw(this, &Player::mouseMoveHandler, 10);
	GameObject::init();
}

void Player::update(float deltaTime)
{
	using namespace DirectX::SimpleMath;
	
	auto cameraForward = mCameraComponent->getForward();
	auto cameraRight = mCameraComponent->getRight();
	auto inputDevice = Game::getInputDevice();
	auto transform = getWorldTransform();
	auto position = transform.getPosition();

	if (inputDevice->IsKeyDown(Keys::D))
	{
		position += mMoveSpeed * deltaTime * cameraRight;
	}
	if (inputDevice->IsKeyDown(Keys::A))
	{
		position -= mMoveSpeed * deltaTime * cameraRight;
	}
	if (inputDevice->IsKeyDown(Keys::W))
	{
		position += mMoveSpeed * deltaTime * cameraForward;
	}
	if (inputDevice->IsKeyDown(Keys::S))
	{
		position -= mMoveSpeed * deltaTime * cameraForward;
	}
	if (inputDevice->IsKeyDown(Keys::Space))
	{
		position += mMoveSpeed * deltaTime * Vector3::Up;
	}
	if (inputDevice->IsKeyDown(Keys::LeftShift))
	{
		position -= mMoveSpeed * deltaTime * Vector3::Up;
	}

	transform.setPosition(position);
	setWorldTransform(transform);

	GameObject::update(deltaTime);
}

void Player::mouseMoveHandler(const SimpleEngine::InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	using namespace DirectX::SimpleMath;
	
	auto transform = getWorldTransform();
	auto rotation = transform.getRotation();

	

	auto deltaYaw = -mouseData.Offset.x * mRotationSpeed;
	auto deltaPitch = -mouseData.Offset.y * mRotationSpeed * 2;

	auto additionalRotation = Quaternion::CreateFromAxisAngle(mCameraComponent->getRight(), deltaPitch) 
		* Quaternion::CreateFromAxisAngle(Vector3::Up, deltaYaw);

	rotation *= additionalRotation;
	transform.setRotation(rotation);

	setWorldTransform(transform);

	mCameraComponent->setFOVAngle(mCameraComponent->getFOVAngle() + mouseData.WheelDelta * mFOVSpeed);	
}
