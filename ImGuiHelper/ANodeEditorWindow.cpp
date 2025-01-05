#include "ANodeEditorWindow.h"
#include "FlowEndNode.h"

using namespace std;
using namespace ImGui;

ANodeEditorWindow::ANodeEditorWindow(const std::string& windowID, bool* openFlag, const size_t& lineCount)
	: AWindow(windowID, false, openFlag), m_lineCount(lineCount)
{
	m_nodesWithLines.resize(lineCount);
}

ANodeEditorWindow::~ANodeEditorWindow()
{
	// m_nodesWithLines는 Canvas에서 해제
}

void ANodeEditorWindow::InitWindow()
{
	m_canvas.RegisterToInteractionManager(this);
	m_flowEndNode = AddNode<FlowEndNode>(m_lineCount - 1);
	m_nodeExecuteContextMenu.m_onExecuteHandler = bind(&FlowEndNode::Execute, m_flowEndNode);
	m_nodeExecuteContextMenu.m_onToggleHoldHandler = [&]() {m_isAdjustNodeLayout = !m_isAdjustNodeLayout; };
	AdjustNodesLayout();
}

void ANodeEditorWindow::RenderWindowImpl()
{
	if (m_isAdjustNodeLayout) AdjustNodesLayout();

	m_canvas.RenderControl();
	m_nodeExecuteContextMenu.DrawNotificator();
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
