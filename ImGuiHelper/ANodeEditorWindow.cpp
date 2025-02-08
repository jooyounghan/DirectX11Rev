#include "ANodeEditorWindow.h"
#include "FlowEndNode.h"

#include <format>

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const std::string& windowID, bool* openFlag, const size_t& lineCount)
	: AWindow(
		windowID, false, openFlag, 
		ImGuiWindowFlags_(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)),
	m_lineCount(lineCount), 
	m_nodeExecuteContextMenu(&m_isEnableAutoPlacement),
	m_messasgeBoxModal("Node Editor Information")
{
	m_canvas.RegisterToInteractionManager(this);

	m_nodesWithLines.resize(lineCount);
	m_flowEndNode = AddNode<FlowEndNode>(m_lineCount - 1);

	m_nodeExecuteContextMenu.m_onExecuteHandler = bind(&FlowEndNode::Execute, m_flowEndNode);
	m_nodeExecuteContextMenu.m_onExecuteSucceed = [&]() { m_messasgeBoxModal.SetMessage("Execute Succeeded"); };
	m_nodeExecuteContextMenu.m_onExecuteFailed = [&](const string& message) { m_messasgeBoxModal.SetMessage(format("Execute Failed {}", message)); };
}

ANodeEditorWindow::~ANodeEditorWindow()
{
	// m_nodesWithLines는 Canvas에서 해제
}

void ANodeEditorWindow::RenderWindowImpl()
{
	if (m_isEnableAutoPlacement) AdjustNodesLayout();

	m_canvas.RenderControl();
	m_nodeExecuteContextMenu.DrawNotificator();
	m_messasgeBoxModal.DrawNotificator();
}

void ANodeEditorWindow::AdjustNodesLayout()
{
	float nextLeft = nodeExternalWidthMargin;
	for (vector<Node*>& nodes : m_nodesWithLines)
	{
		float currentLeft = nextLeft;
		float nextTop = nodeExternalHeightMargin;
		for (Node* node : nodes)
		{
			float currentTop = nextTop;
			node->SetLeftTop(ImVec2(currentLeft, currentTop));
			const ImVec2 nodeTotalSize = node->GetTotalSize();

			nextTop += nodeTotalSize.y + nodeExternalHeightMargin;
			nextLeft = std::max(nextLeft, currentLeft + nodeTotalSize.x + nodeExternalWidthMargin);
		}
	}
}
