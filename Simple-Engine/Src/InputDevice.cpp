#include "pch.h"
#include "InputDevice.h"
#include <iostream>
#include "Game.h"


using namespace DirectX::SimpleMath;

using namespace SimpleEngine;

InputDevice::InputDevice() : mGame(Game::getInstance())
{
	keys = std::make_unique<std::unordered_set<Keys>>();
	
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = mGame.lock()->getHWnd();

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = mGame.lock()->getHWnd();

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{
		auto errorCode = GetLastError();
		std::cout << "ERROR: " << errorCode << std::endl;
	}
}

InputDevice::~InputDevice()
{
	
}

void InputDevice::OnKeyDown(KeyboardInputEventArgs args)
{
	bool Break = args.Flags & 0x01;

	auto key = static_cast<Keys>(args.VKey);

	if (args.MakeCode == 42) key = Keys::LeftShift;
	if (args.MakeCode == 54) key = Keys::RightShift;
	
	if(Break) {
		if(keys->count(key))	RemovePressedKey(key);
	} else {
		if (!keys->count(key))	AddPressedKey(key);
	}
}

void InputDevice::OnMouseMove(RawMouseEventArgs args)
{
	if(args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonDown))
		AddPressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonUp))
		RemovePressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonDown))
		AddPressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonUp))
		RemovePressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonDown))
		AddPressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonUp))
		RemovePressedKey(Keys::MiddleButton);

	POINT p;
	GetCursorPos(&p);
	auto game = mGame.lock();
	if (game) {
		ScreenToClient(game->getHWnd(), &p);

		MousePosition = Vector2(p.x, p.y);
		MouseOffset = Vector2(args.X, args.Y);
		MouseWheelDelta = args.WheelDelta;

		const MouseMoveEventArgs moveArgs = { MousePosition, MouseOffset, MouseWheelDelta };

		//printf(" Mouse: posX=%04.4f posY:%04.4f offsetX:%04.4f offsetY:%04.4f, wheelDelta=%04d \n",
		//	MousePosition.x,
		//	MousePosition.y,
		//	MouseOffset.x,
		//	MouseOffset.y,
		//	MouseWheelDelta);

		MouseMove.Broadcast(moveArgs);
	}
}

void InputDevice::AddPressedKey(Keys key)
{
	//if (!mGame->isActive) {
	//	return;
	//}
	keys->insert(key);
}

void InputDevice::RemovePressedKey(Keys key)
{
	keys->erase(key);
}

bool InputDevice::IsKeyDown(Keys key)
{
	return keys->count(key);
}

