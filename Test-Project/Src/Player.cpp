#include "pch.h"
#include "Player.h"
#include "CameraComponent.h"

using namespace SimpleEngine;

void Player::init()
{
	mCameraComponent = createComponent<CameraComponent>();
	GameObject::init();
}
