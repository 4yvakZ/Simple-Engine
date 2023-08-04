#pragma once
#include "Component.h"
#include "Game.h"
#include "RenderSystem.h"

namespace SimpleEngine
{
	class Component;
	class RenderComponent;

	class GameObject
	{
	public:
		explicit GameObject(std::shared_ptr<GameObject> parent = nullptr) :
			mParent(parent) 
		{}
			
		GameObject(const GameObject&) = default;
		GameObject& operator=(const GameObject&) = default;

		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		virtual ~GameObject() = default;

		virtual void update(float deltaTime);
		virtual void init();

		auto parent() {
			return mParent;
		}

		void setParent(std::shared_ptr<GameObject> parent) {
			mParent = parent;
		}

	private:
		std::shared_ptr<GameObject> mParent;
		std::vector<std::shared_ptr<Component>> mComponents;

	protected:
		template<class T, class ...Ts, class = std::enable_if_t<
			std::is_base_of<
			std::decay_t<Component>, T>::value>>
			std::shared_ptr<T> createComponent(Ts&&...args);
	};

	template<class T, class ...Ts, class>
	std::shared_ptr<T> GameObject::createComponent(Ts && ...args)
	{
		auto component = std::make_shared<T>(std::forward<Ts>(args)...);
		mComponents.emplace_back(component);
		if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
			Game::getRenderSystem()->addRenderComponent(renderComponent);
		}
		return component;
	}
}