#pragma once
#include "GameObject.h"

class Player : public SimpleEngine::GameObject
{
public:
	virtual void init() override;
private:
	std::shared_ptr<SimpleEngine::CameraComponent> mCameraComponent;
};

