#pragma once
#include "AApplication.h"

class GameEngine : public App::AApplication
{
public:
	GameEngine();

protected:
	virtual void Update(const float& deltaTime) override;
	virtual void AppProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

};