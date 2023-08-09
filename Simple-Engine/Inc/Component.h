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

		virtual void init() = 0;

		virtual void update() = 0;

		virtual void construct();

		void setOwner(std::shared_ptr<GameObject> owner);

		std::shared_ptr<GameObject> getOwner() const;

	private:
		std::weak_ptr<GameObject> mOwner;
	};
}
