#pragma once
#include <list>
#include <functional>

class MouseEvent
{
private:
	std::list<std::function<void(const float&, const float&)>> MouseEventHandlers;

public:
	void AddEventHandler(const std::function<void(const float&, const float&)>& EventHandler);

	template<typename Base, typename Derived>
	void AddEventHandler(void(Base::* MemberFunc)(const float&, const float&), Derived* instance);

public:
	void RunEventHandlers(const float& NdcX, const float& NdcY);
};

template<typename Base, typename Derived>
inline void MouseEvent::AddEventHandler(void(Base::* MemberFunc)(const float&, const float&), Derived* instance)
{
	static_assert(std::is_base_of<Base, Derived>::value, "Function Don't Belongs To The Instance");
	KeyEventHandlers.push_back(std::bind(MemberFunc, instance, std::placeholders::_1, std::placeholders::_2));
}
