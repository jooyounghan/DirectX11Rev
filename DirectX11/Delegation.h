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
	void operator +=(std::function<void(Args...)> Function)
	{
		DelegatedFunctions.push_back(Function);
	}

	void operator -=(std::function<void(Args...)> Function)
	{
		DelegatedFunctions.remove(Function);
	}

	void Execute(Args... args)
	{
		for (auto& DelegatedFunction : DelegatedFunctions)
		{
			DelegatedFunction(args...);
		}
	}
};

