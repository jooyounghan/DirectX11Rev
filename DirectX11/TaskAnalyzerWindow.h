#pragma once
#include "AWindow.h"
#include "HeaderHelper.h"
#include <unordered_map>
#include <string>

enum class RenderingAlgorithm
{
	ForwardRendering,
	DefferedRendergin
};

class TaskAnalyzerWindow : public AWindow
{
public:
	TaskAnalyzerWindow();
	virtual ~TaskAnalyzerWindow();

private:
	static std::unordered_map<RenderingAlgorithm, std::string> RenderingAlgorithmToDescriptions;

public:
	virtual void RenderWindow() override;

private:
	RenderingAlgorithm SelectedRenderingAlgorithm = RenderingAlgorithm::ForwardRendering;
	MakeGetter(SelectedRenderingAlgorithm);

private:
	void DrawFPSLinePlotting();
	void DrawTotalIndexedCount();
	void DrawRenderingAlgorithmSelection();
};

