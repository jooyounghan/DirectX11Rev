#pragma once
#include "AWindow.h"
#include "PerformanceAnalyzer.h"

class PerformanceAnalyzerWindow : public AWindow
{
public:
	PerformanceAnalyzerWindow(const std::string& windowID);
	~PerformanceAnalyzerWindow() override = default;

public:
	virtual void PrepareWindow() override;

protected:
	virtual void RenderWindowImpl() override;

private:
	void DrawFPSLinePlotting();
	void DrawPerformanceIndicators();
};

