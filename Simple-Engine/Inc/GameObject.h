#pragma once
#include "Component.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Transform.h"

namespace SimpleEngine
{
	class Component;
	class RenderComponent;
	class CameraComponent;

	class GameObject : public std::enable_shared_from_this<GameObject>
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

		auto parent() 
		{
			return mParent.lock();
		}

		void setParent(std::shared_ptr<GameObject> parent) 
		{
			mParent = parent;
		}

		void setTransform(const Transform& transform)
		{
			mTransform = transform;
		}

		const Transform& getTransform() const
		{
			return mTransform;
		}

		Transform getWorldTransform() const;

	private:
		std::weak_ptr<GameObject> mParent;
		std::vector<std::shared_ptr<Component>> mComponents;
		Transform mTransform;

	protected:
		template<class T, class ...Ts, class = std::enable_if_t<
			std::is_base_of_v<
			std::decay_t<Component>, T>>>
			std::shared_ptr<T> createComponent(Ts&&...args);
	};

	template<class T, class ...Ts, class>
	std::shared_ptr<T> GameObject::createComponent(Ts && ...args)
	{
		auto component = std::make_shared<T>(std::forward<Ts>(args)...);

		mComponents.emplace_back(component);
		component->setOwner(shared_from_this());
		component->construct();
		return component;
	}
}