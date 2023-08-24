#pragma once

namespace SimpleEngine
{
	class Component;
	class RenderComponent;
	class RenderSystem;
	class InputDevice;
	class GameObject;
	class CameraComponent;

	class Game
	{

	public:
		Game();

		explicit Game(int ClientWidth, int ClientHeight);

		Game& operator=(const Game&) = delete;
		Game(const Game&) = delete;

		virtual ~Game() = default;

		virtual LRESULT MsgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

		void Run();

		void Exit();


		HWND GetHWnd() const;

		std::shared_ptr<CameraComponent> GetActiveCameraComponent() const;
		void SetActiveCameraComp(std::shared_ptr<CameraComponent> comp);

		int ClientWidth();
		int ClientHeight();

	protected:

		virtual void Init();

		virtual void PrepareResources();

		virtual void UpdateInternal();

		virtual void Update(float deltaTime);

		virtual void Draw();

		virtual void DestroyResources();

		void CreateAppWindow();
	private:

#if defined(DEBUG) || defined(_DEBUG)
		static inline bool isCreating = false;
#endif // defined(DEBUG) || defined(_DEBUG)

		static inline std::shared_ptr<Game> mInstance = nullptr;

		int mClientWidth;
		int mClientHeight;

		HWND hWnd = nullptr;

		bool mIsExitRequested = false;

		std::chrono::time_point<std::chrono::steady_clock> mPrevTime;
		float mTotalTime = 0;
		float mDeltaTime = 0;

		//std::unique_ptr<GameObject> RootObject;

		std::weak_ptr<CameraComponent> mActiveCameraComp;

		static inline std::shared_ptr<RenderSystem> sRenderSystem = nullptr;

		static inline std::shared_ptr<InputDevice> sInputDevice = nullptr;

		static inline std::vector<std::weak_ptr<GameObject>> sGameObjects = std::vector<std::weak_ptr<GameObject>>();

	public:
		template<class T, class...Ts, class = std::enable_if_t<
			std::is_base_of_v<
			std::decay_t<Game>, T>>>
			static std::shared_ptr<Game> CreateGameInstance(Ts&&... args);

		template<class T, class ...Ts, class = std::enable_if_t<
			std::is_base_of_v<
			std::decay_t<GameObject>, T>>>
			static std::shared_ptr<T> CreateGameObject(Ts&&...args);

		static std::shared_ptr<Game> GetInstance();
		static std::shared_ptr<SimpleEngine::RenderSystem> GetRenderSystem();
		static std::shared_ptr<SimpleEngine::InputDevice> GetInputDevice();
	};
	
	template<class T, class ...Ts, class>
	std::shared_ptr<Game> Game::CreateGameInstance(Ts&& ...Args)
	{
#if defined(DEBUG) || defined(_DEBUG)
		isCreating = true;
#endif // defined(DEBUG) || defined(_DEBUG)
		assert(("Only one instance of game can be created", mInstance == nullptr));
		mInstance = std::make_shared<T>(std::forward<Ts>(Args)...);
		mInstance->PrepareResources();
		return mInstance;
	}

	template<class T, class ...Ts, class>
	std::shared_ptr<T> Game::CreateGameObject(Ts && ...args)
	{
		assert(("Game instance have not been created", mInstance));
		auto gameObject = std::make_shared<T>(std::forward<Ts>(args)...);
		sGameObjects.emplace_back(gameObject);
		return gameObject;
	}
}

