#pragma once
#include <list>
#include <functional>

class KeyEvent
{
private:
	std::list<std::function<void(const float&)>> KeyEventHandlers;

public:
	void AddEventHandler(const std::function<void(const float&)>& EventHandler);

	template<typename Base, typename Derived>
	void AddEventHandler(void(Base::* MemberFunc)(const float&), Derived* instance);

public:
	void RunEventHandlers(const float& DeltaTime);
};

template<typename Base, typename Derived>
inline void KeyEvent::AddEventHandler(void(Base::* MemberFunc)(const float&), Derived* instance)
{
	static_assert(std::is_base_of<Base, Derived>::value, "Function Don't Belongs To The Instance");
	KeyEventHandlers.push_back(std::bind(MemberFunc, instance, std::placeholders::_1));
}
