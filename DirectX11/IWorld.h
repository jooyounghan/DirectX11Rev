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
	virtual void RenderWorld() = 0;

public:
	virtual LRESULT AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

