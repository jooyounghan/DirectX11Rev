#pragma once
#include "AWindow.h"
#include "Canvas.h"
#include "NodeExecuteContextMenu.h"

class FlowEndNode;

class ANodeEditorWindow : public AWindow
{
public:
	ANodeEditorWindow(const std::string& windowID, bool* openFlag);
	~ANodeEditorWindow() override;

protected:
	virtual void RenderWindowImpl() override;

protected:
	FlowEndNode* m_flowEndNode;
	NodeExecuteContextMenu m_nodeExecuteContextMenu;

protected:
	Canvas m_canvas;
};

