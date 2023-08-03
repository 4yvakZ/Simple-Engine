#include "pch.h"
#include "Game.h"
#include "RenderSystem.h"
#include "GameObject.h"

using namespace SimpleEngine;

#if defined(DEBUG) || defined(_DEBUG)
bool Game::isCreating = false;
#endif // defined(DEBUG) || defined(_DEBUG)

std::shared_ptr<Game> Game::mInstance = nullptr;
std::shared_ptr<RenderSystem> Game::mRenderSystem = nullptr;

std::vector<std::weak_ptr<GameObject>> Game::mGameObjects = std::vector<std::weak_ptr<GameObject>>();

LRESULT MainWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return SimpleEngine::Game::getInstance()->MsgProc(hwnd, umessage, wparam, lparam);
}

LRESULT CALLBACK SimpleEngine::Game::MsgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		exit();
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
			printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey);

			//inputDevice->OnKeyDown({
			//	raw->data.keyboard.MakeCode,
			//	raw->data.keyboard.Flags,
			//	raw->data.keyboard.VKey,
			//	raw->data.keyboard.Message
			//	});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);

			//inputDevice->OnMouseMove({
			//	raw->data.mouse.usFlags,
			//	raw->data.mouse.usButtonFlags,
			//	static_cast<int>(raw->data.mouse.ulExtraInformation),
			//	static_cast<int>(raw->data.mouse.ulRawButtons),
			//	static_cast<short>(raw->data.mouse.usButtonData),
			//	raw->data.mouse.lLastX,
			//	raw->data.mouse.lLastY
			//	});
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

SimpleEngine::Game::Game(int clientWidth, int clientHeight)
	: mClientWidth(clientWidth), mClientHeight(clientHeight)//, hWnd(nullptr)
{
#if defined(DEBUG) || defined(_DEBUG)
	assert(("To create game instance use createGameInstance template", isCreating));
#endif // defined(DEBUG) || defined(_DEBUG)
}

void SimpleEngine::Game::init()
{
	for (auto it = mGameObjects.begin(); it < mGameObjects.end(); it++)
	{
		auto gameObject = it->lock();
		if (gameObject) 
		{
			gameObject->init();
		}
	}
}

void SimpleEngine::Game::prepareResources()
{
	createWindow();

	mRenderSystem = std::make_shared<RenderSystem>(hWnd, mClientWidth, mClientHeight);
}

void SimpleEngine::Game::update()
{
}

void SimpleEngine::Game::draw()
{
	mRenderSystem->prepareFrame();

	mRenderSystem->draw();

	mRenderSystem->endFrame();
}

void SimpleEngine::Game::updateInternal()
{
}

void SimpleEngine::Game::destroyResources()
{
}

void SimpleEngine::Game::createWindow()
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

void SimpleEngine::Game::run()
{
	prepareResources();

	init();

	prevTime = std::chrono::steady_clock::now();
	totalTime = 0;
	unsigned int frameCount = 0;

	MSG msg;

	while (!isExitRequested)
	{
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				isExitRequested = true;
			}
		}



		auto	curTime = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
		prevTime = curTime;

		totalTime += deltaTime;
		frameCount++;

		if (totalTime > 1.0f)
		{
			float fps = frameCount / totalTime;

			totalTime -= 1.0f;

			WCHAR text[256];
			swprintf_s(text, TEXT("FPS: %f"), fps);
			SetWindowText(hWnd, text);

			frameCount = 0;
		}

		updateInternal();

		update();

		draw();

	}

	destroyResources();
}

void SimpleEngine::Game::exit()
{
}


