#pragma once
#include <functional>

class AImGuiWindow
{
public:
	virtual void ShowWindow() = 0;

public:
	std::function<void(AImGuiWindow*)> OnClose = [&](AImGuiWindow* window) {};
};

