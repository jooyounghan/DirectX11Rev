#pragma once
#include "AWindow.h"

class TaskAnalyzerWindow : public AWindow
{
public:
	TaskAnalyzerWindow();
	virtual ~TaskAnalyzerWindow();

public:
	virtual void RenderWindow() override;

private:
	void DrawFPSLinePlotting();
};

