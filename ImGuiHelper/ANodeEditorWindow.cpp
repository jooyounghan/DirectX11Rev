#include "ANodeEditorWindow.h"
#include "FlowEndNode.h"
#include "NodeConstant.h"

using namespace std;
using namespace ImGui;

template<size_t NodeLineCount>
ANodeEditorWindow<NodeLineCount>::ANodeEditorWindow(const std::string& windowID, bool* openFlag)
	: AWindow(windowID, false, openFlag)
{
	m_canvas.RegisterToInteractionManager(this);
	m_flowEndNode = AddNode<NodeLineCount - 1, FlowEndNode>(defaultPortRadius);
	m_nodeExecuteContextMenu.m_onExecuteHandler = bind(&FlowEndNode::Execute, m_flowEndNode);
}

template<size_t NodeLineCount>
ANodeEditorWindow<NodeLineCount>::~ANodeEditorWindow()
{
	for (std::vector<Node*>& nodes : m_nodesWithLines)
	{
		for (Node* node : nodes)
		{
			delete node;
		}
	}
}

template<size_t NodeLineCount>
void ANodeEditorWindow<NodeLineCount>::RenderWindowImpl()
{
	m_canvas.RenderControl();
	m_nodeExecuteContextMenu.DrawNotificator();
}

template<size_t NodeLineCount>
void ANodeEditorWindow<NodeLineCount>::AdjustNodesLayout()
{
	float nextLeft = nodeExternalWidthMargin;
	for (vector<Node*>& nodes : m_nodesWithLines)
	{
		float currentLeft = nextLeft;
		float nextTop = nodeExternalHeightMargin;
		for (Node* node : nodes)
		{
			node->UpdateNodeSize();

			float currentTop = nextTop;
			node->SetLeftTop(ImVec2(currentLeft, currentTop));
			
			
			const ImVec2 nodeTotalSize = node->GetTotalSize();

			nextTop += nodeTotalSize.y + nodeExternalHeightMargin;
			nextLeft = std::max(nextLeft, currentLeft + nodeTotalSize.x + nodeExternalWidthMargin);
		}
	}
}

template class ANodeEditorWindow<1>;
template class ANodeEditorWindow<2>;
template class ANodeEditorWindow<3>;
template class ANodeEditorWindow<4>;
template class ANodeEditorWindow<5>;
template class ANodeEditorWindow<6>;
template class ANodeEditorWindow<7>;
template class ANodeEditorWindow<8>;
template class ANodeEditorWindow<9>;
template class ANodeEditorWindow<10>;
