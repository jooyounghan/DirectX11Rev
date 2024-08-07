#pragma once
#include "IWindow.h"

class TaskAnalyzerWindow : public IWindow
{
public:
	TaskAnalyzerWindow();
	virtual ~TaskAnalyzerWindow();

public:
	virtual void RenderWindow() override;

private:
	void DrawFPSLinePlotting();
};

