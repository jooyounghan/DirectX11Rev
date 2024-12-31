#pragma once
#include "AWindow.h"
#include "Canvas.h"

class ANodeEditorWindow : public AWindow
{
public:
	ANodeEditorWindow(const std::string& windowID);

public:
	virtual void PrepareWindow() override {};

private:
	virtual void RenderWindowImpl() override;

protected:
	Canvas m_canvas;
};

