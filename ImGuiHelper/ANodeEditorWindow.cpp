#include "ANodeEditorWindow.h"
#include "FlowEndNode.h"


using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const string& windowID, bool* openFlag)
	: AWindow(windowID, false, openFlag)
{
	m_canvas.RegisterToInteractionManager(this);

	m_flowEndNode = new FlowEndNode(ImVec2(0.f, 0.f), 10.f, m_canvas.GetOriginPosition());

	m_flowEndNode->AddToDrawElementManager(&m_canvas);
	m_flowEndNode->RegisterToInteractionManager(&m_canvas);

	m_nodeExecuteContextMenu.m_onExecuteHandler = bind(&FlowEndNode::Execute, m_flowEndNode);
}

ANodeEditorWindow::~ANodeEditorWindow()
{
	delete m_flowEndNode;
}

void ANodeEditorWindow::RenderWindowImpl()
{
	m_canvas.RenderControl();
	m_nodeExecuteContextMenu.DrawNotificator();
}
