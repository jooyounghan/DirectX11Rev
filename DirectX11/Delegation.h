#pragma once
#include <functional>
#include <list>

template <typename ...Args>
class Delegation
{
public:
	Delegation() {};
	~Delegation() {};

protected:
	std::list<std::function<void(Args...)>> DelegatedFunctions;

public:
	void operator +=(const std::function<void(Args...)>& Function)
	{
		DelegatedFunctions.push_back(Function);
	}

	void operator -=(const std::function<void(Args...)>& Function)
	{
		DelegatedFunctions.remove_if(
			[&](const std::function<void(Args...)>& _Other) { return &_Other == &Function; }
		);
	}

	void Invoke(Args... args)
	{
		for (auto& DelegatedFunction : DelegatedFunctions)
		{
			DelegatedFunction(args...);
		}
	}
};

