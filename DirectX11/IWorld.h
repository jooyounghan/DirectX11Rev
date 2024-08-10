#pragma once
#include <Windows.h>

class GraphicsPipeline;

class IWorld
{
public:
	IWorld(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~IWorld();

public:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;

public:
	virtual void UpdateWorld(const float& DeltaTimeIn) = 0;
	virtual void RenderWorld() = 0;

public:
	virtual void AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void ManageMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

