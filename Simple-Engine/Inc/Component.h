#pragma once

namespace SimpleEngine
{
	class GameObject;
	class Component : public std::enable_shared_from_this<Component>
	{
	public:
		Component() = default;

		Component(const Component&) = default;
		Component& operator=(const Component&) = default;

		Component(Component&&) = default;
		Component& operator=(Component&&) = default;

		virtual ~Component() {}

		virtual void Init() = 0;

		virtual void Update() = 0;

		virtual void OnConstructed();

		void SetOwner(std::shared_ptr<GameObject> owner);

		std::shared_ptr<GameObject> GetOwner() const;

	private:
		std::weak_ptr<GameObject> mOwner;
	};
}
