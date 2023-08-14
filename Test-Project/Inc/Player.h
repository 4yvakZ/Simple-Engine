#pragma once
#include "GameObject.h"
#include "InputDevice.h"

class Player : public SimpleEngine::GameObject
{
public:
	virtual void init() override;
	virtual void update(float deltaTime) override;
private:
	void mouseMoveHandler(const SimpleEngine::InputDevice::MouseMoveEventArgs& mouseData, int payload);
	std::shared_ptr<SimpleEngine::CameraComponent> mCameraComponent;
	float mMoveSpeed = 10.0f;
	float mFOVSpeed = 0.001f;
	float mRotationSpeed = 0.005f;
};

