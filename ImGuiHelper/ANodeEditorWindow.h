#pragma once
#include "Node.h"
#include "Canvas.h"
#include "AWindow.h"
#include "NodeExecuteContextMenu.h"

#include <array>

class FlowEndNode;

template<size_t NodeLineCount>
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
	std::array<std::vector<Node*>, NodeLineCount> m_nodesWithLines;
	Canvas m_canvas;

public:
	void AdjustNodesLayout();

public:
	template<size_t LineIndex, typename T, typename... Args>
	T* AddNode(Args&&... args);
};

template<size_t NodeLineCount>
template<size_t LineIndex, typename T, typename ...Args>
inline T* ANodeEditorWindow<NodeLineCount>::AddNode(Args && ...args)
{
	static_assert(std::is_base_of_v<Node, T>);
	static_assert(LineIndex < NodeLineCount);
	T* addedNode = new T(ImVec2(0.f, 0.f), std::forward<Args>(args)..., m_canvas.GetOriginPosition());
	addedNode->AddToDrawElementManager(&m_canvas);
	addedNode->RegisterToInteractionManager(&m_canvas);
	m_nodesWithLines[LineIndex].emplace_back(addedNode);
	
	return addedNode;
}
