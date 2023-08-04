#pragma once

namespace SimpleEngine
{
	class Component;
	class RenderComponent;
	class RenderSystem;
	class GameObject;

	class Game
	{

	public:
		Game();

		explicit Game(int clientWidth, int clientHeight);

		Game& operator=(const Game&) = delete;
		Game(const Game&) = delete;

		virtual ~Game() = default;

		virtual LRESULT MsgProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

		void run();

		void exit();

	protected:

		virtual void init();

		virtual void prepareResources();

		virtual void updateInternal();

		virtual void update();

		virtual void draw();

		virtual void destroyResources();

		void createWindow();
	private:

#if defined(DEBUG) || defined(_DEBUG)
		static bool isCreating;
#endif // defined(DEBUG) || defined(_DEBUG)

		static std::shared_ptr<Game> mInstance;

		int mClientWidth;
		int mClientHeight;

		HWND hWnd = nullptr;

		bool isExitRequested = false;

		std::chrono::time_point<std::chrono::steady_clock> prevTime;
		float totalTime;
		float deltaTime;

		std::unique_ptr<GameObject> RootObject;

		static std::shared_ptr<RenderSystem> mRenderSystem;

		static std::vector<std::weak_ptr<GameObject>> mGameObjects;
	public:
		template<class T, class...Ts, class = std::enable_if_t<
			std::is_base_of<
			std::decay_t<Game>, T>::value>>
			static std::shared_ptr<Game> createGameInstance(Ts&&... args);

		template<class T, class ...Ts, class = std::enable_if_t<
			std::is_base_of<
			std::decay_t<GameObject>, T>::value>>
			static std::shared_ptr<T> createGameObject(Ts&&...args);

		static std::shared_ptr<Game> getInstance()
		{
			return mInstance;
		}

		static std::shared_ptr<SimpleEngine::RenderSystem> getRenderSystem()
		{
			return mRenderSystem;
		}
	};
	
	template<class T, class ...Ts, class>
	std::shared_ptr<Game> Game::createGameInstance(Ts && ...Args)
	{
#if defined(DEBUG) || defined(_DEBUG)
		isCreating = true;
#endif // defined(DEBUG) || defined(_DEBUG)
		assert(("Only one instance of game can be created", mInstance == nullptr));
		mInstance = std::make_shared<T>(std::forward<Ts>(Args)...);
		return mInstance;
	}

	template<class T, class ...Ts, class>
	std::shared_ptr<T> Game::createGameObject(Ts && ...args)
	{
		assert(("Game instance have not been created", mInstance));
		auto gameObject = std::make_shared<T>(std::forward<Ts>(args)...);
		mGameObjects.emplace_back(gameObject);
		return gameObject;
	}
}
