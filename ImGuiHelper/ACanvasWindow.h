#pragma once
#include "AWindow.h"
#include "Canvas.h"

class ACanvasWindow : public AWindow
{
public:
	ACanvasWindow(const std::string& windowID);

public:
	virtual void PrepareWindow() override {};

private:
	virtual void RenderWindowImpl() override;

protected:
	Canvas m_canvas;
};

