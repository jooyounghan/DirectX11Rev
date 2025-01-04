#pragma once
#include "AWindow.h"
#include "Canvas.h"

class ANodeEditorWindow : public AWindow
{
public:
	ANodeEditorWindow(const std::string& windowID, bool* openFlag);
	~ANodeEditorWindow() override = default;

protected:
	virtual void RenderWindowImpl() override;

//protected:
//	FlowEndNode m_flowEndNode;

protected:
	Canvas m_canvas;
};

