#include "pch.h"
#include "Game.h"
#include "RenderSystem.h"
#include "GameObject.h"
#include "InputDevice.h"
#include "CameraComponent.h"
#include "Utils.h"
#include "AssetManager.h"
#include "DebugRenderer.h"

using namespace SimpleEngine;
using namespace DirectX::SimpleMath;

LRESULT MainWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return SimpleEngine::Game::GetInstance()->MsgProc(hwnd, umessage, wparam, lparam);
}

LRESULT CALLBACK SimpleEngine::Game::MsgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		Exit();
		return 0;
	}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.Reserved,
			//	raw->data.keyboard.ExtraInformation,
			//	raw->data.keyboard.Message,
			//	raw->data.keyboard.VKey);

			sInputDevice->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);

			sInputDevice->OnMouseMove({
				raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}

		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

SimpleEngine::Game::Game():
	Game(1200, 800)
{

}

SimpleEngine::Game::Game(int ClientWidth, int ClientHeight)
	: mClientWidth(ClientWidth), mClientHeight(ClientHeight)//, hWnd(nullptr)
{
#if defined(DEBUG) || defined(_DEBUG)
	assert(("To create game instance use createGameInstance template", isCreating));
#endif // defined(DEBUG) || defined(_DEBUG)
}

void SimpleEngine::Game::Init()
{
	AssetManager::GetInstance()->Init();

	//DebugRenderer::GetInstance()->Init(sRenderSystem->getDevice());

	for (auto it = sGameObjects.begin(); it < sGameObjects.end();)
	{
		auto gameObject = it->lock();
		if (gameObject)
		{
			gameObject->Init();
			it++;
		}
		else
		{
			it = sGameObjects.erase(it);
		}
	}
}

void SimpleEngine::Game::PrepareResources()
{
	CreateAppWindow();

	sRenderSystem = std::make_shared<RenderSystem>(hWnd, mClientWidth, mClientHeight);

	sInputDevice = std::make_shared<InputDevice>();
}

void SimpleEngine::Game::Update(float deltaTime)
{
	for (auto it = sGameObjects.begin(); it < sGameObjects.end();)
	{
		auto gameObject = it->lock();
		if (gameObject)
		{
			gameObject->Update(deltaTime);
			it++;
		}
		else
		{
			it = sGameObjects.erase(it);
		}
	}
}

void SimpleEngine::Game::Draw()
{
	sRenderSystem->PrepareFrame();

	sRenderSystem->Draw();

	sRenderSystem->EndFrame();
}

void SimpleEngine::Game::UpdateInternal()
{
	mIsExitRequested = sInputDevice->IsKeyDown(Keys::Escape);

	FrameConstBufferData frameConstBufferData = {};
	if (auto camera = mActiveCameraComp.lock()) {
		frameConstBufferData.mCameraPos = ToVector4(camera->GetWorldTransform().GetPosition(), 1);
		frameConstBufferData.mViewProjection = camera->GetViewProjection();
	} 
	sRenderSystem->Update(mDeltaTime, frameConstBufferData);

	if (sInputDevice->IsKeyDown(Keys::F1)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::None);
	}
	if (sInputDevice->IsKeyDown(Keys::F2)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::WorldPosition);
	}
	if (sInputDevice->IsKeyDown(Keys::F3)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::Normals);
	}
	if (sInputDevice->IsKeyDown(Keys::F4)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::Albedo);
	}
	if (sInputDevice->IsKeyDown(Keys::F5)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::MetallicRoughnessAO);
	}
	if (sInputDevice->IsKeyDown(Keys::F6)) {
		sRenderSystem->SetDebugFlag(RenderSystem::DebugFlag::Lighting);
	}
}

void SimpleEngine::Game::DestroyResources()
{
}

void SimpleEngine::Game::CreateAppWindow()
{
	LPCWSTR applicationName = L"My3DApp";
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);
	RECT windowRect = { 0, 0, static_cast<LONG>(mClientWidth), static_cast<LONG>(mClientHeight)};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - mClientWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - mClientHeight) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);
}

std::shared_ptr<Game> SimpleEngine::Game::GetInstance()
{
	return mInstance;
}

std::shared_ptr<SimpleEngine::RenderSystem> SimpleEngine::Game::GetRenderSystem()
{
	return sRenderSystem;
}

std::shared_ptr<SimpleEngine::InputDevice> SimpleEngine::Game::GetInputDevice()
{
	return sInputDevice;
}

void SimpleEngine::Game::Run()
{
	//PrepareResources();

	Init();

	mPrevTime = std::chrono::steady_clock::now();
	mTotalTime = 0;
	unsigned int frameCount = 0;

	MSG msg;

	while (!mIsExitRequested)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// If windows signals to end the application then Exit out.
			if (msg.message == WM_QUIT)
			{
				mIsExitRequested = true;
			}
		}



		auto	curTime = std::chrono::steady_clock::now();
		mDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - mPrevTime).count() / 1000000.0f;
		mPrevTime = curTime;

		mTotalTime += mDeltaTime;
		frameCount++;

		if (mTotalTime > 1.0f)
		{
			float fps = frameCount / mTotalTime;

			mTotalTime -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(hWnd, text);

			frameCount = 0;
		}

		UpdateInternal();

		Update(mDeltaTime);

		Draw();

	}

	DestroyResources();
}

void SimpleEngine::Game::Exit()
{
}

HWND SimpleEngine::Game::GetHWnd() const
{
	return hWnd;
}

std::weak_ptr<CameraComponent> SimpleEngine::Game::GetActiveCameraComponent() const
{
	return mActiveCameraComp;
}

void SimpleEngine::Game::SetActiveCameraComp(std::shared_ptr<CameraComponent> comp) {
	mActiveCameraComp = comp;
}

int SimpleEngine::Game::ClientWidth() {
	return mClientWidth;
}

int SimpleEngine::Game::ClientHeight() {
	return mClientHeight;
}


