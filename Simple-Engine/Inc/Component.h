#pragma once

namespace SimpleEngine
{
	class Component
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
	};
}
