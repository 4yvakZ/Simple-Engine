#include "pch.h"
#include "Player.h"
#include "CameraComponent.h"

using namespace SimpleEngine;

void Player::Init()
{
	using namespace DirectX::SimpleMath;
	mCameraComponent = CreateComponent<CameraComponent>();
	auto camTransform = mCameraComponent->GetTransform();
	camTransform.SetPosition(Vector3(0, 0, 0));
	mCameraComponent->SetTransform(camTransform);
	Game::GetInputDevice()->MouseMove.AddRaw(this, &Player::mouseMoveHandler, 10);
	GameObject::Init();
}

void Player::Update(float deltaTime)
{
	using namespace DirectX::SimpleMath;
	
	auto cameraForward = mCameraComponent->GetForward();
	auto cameraRight = mCameraComponent->GetRight();
	auto inputDevice = Game::GetInputDevice();
	auto transform = GetWorldTransform();
	auto position = transform.GetPosition();

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

	transform.SetPosition(position);
	SetWorldTransform(transform);

	GameObject::Update(deltaTime);
}

void Player::mouseMoveHandler(const SimpleEngine::InputDevice::MouseMoveEventArgs& mouseData, int payload)
{
	using namespace DirectX::SimpleMath;
	
	auto transform = GetWorldTransform();
	auto rotation = transform.GetRotation();

	

	auto deltaYaw = -mouseData.Offset.x * mRotationSpeed;
	auto deltaPitch = -mouseData.Offset.y * mRotationSpeed * 2;

	auto additionalRotation = Quaternion::CreateFromAxisAngle(mCameraComponent->GetRight(), deltaPitch) 
		* Quaternion::CreateFromAxisAngle(Vector3::Up, deltaYaw);

	rotation *= additionalRotation;
	transform.SetRotation(rotation);

	SetWorldTransform(transform);

	mCameraComponent->SetFOVAngle(mCameraComponent->GetFOVAngle() + mouseData.WheelDelta * mFOVSpeed);	
}
