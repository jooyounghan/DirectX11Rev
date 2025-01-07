#pragma once
#include "Node.h"
#include "Canvas.h"
#include "AWindow.h"
#include "NodeExecuteContextMenu.h"
#include "NodeConstant.h"

#include <array>

struct ID3D11Device;
struct ID3D11DeviceContext;

class FlowEndNode;

class ANodeEditorWindow : public AWindow
{
public:
	ANodeEditorWindow(const std::string& windowID, bool* openFlag, const size_t& lineCount);
	~ANodeEditorWindow() override;

protected:
	virtual void PrepareWindow() override {};
	virtual void RenderWindowImpl() override;
	
protected:
	size_t m_lineCount;
	FlowEndNode* m_flowEndNode;
	NodeExecuteContextMenu m_nodeExecuteContextMenu;

protected:
	std::vector<std::vector<Node*>> m_nodesWithLines;
	Canvas m_canvas;
	bool m_isEnableAutoPlacement = true;

public:
	void AdjustNodesLayout();

public:
	template<typename T, typename... Args>
	T* AddNode(const size_t& lineIndex, Args&&... args);
};

template<typename T, typename ...Args>
inline T* ANodeEditorWindow::AddNode(const size_t& lineIndex, Args && ...args)
{
	static_assert(std::is_base_of_v<Node, T>);
	T* addedNode = new T(ImVec2(0.f, 0.f), defaultPortRadius, m_canvas.GetOriginPosition(), std::forward<Args>(args)...);
	m_canvas.AddDrawElement(addedNode);
	addedNode->RegisterToInteractionManager(&m_canvas);
	m_nodesWithLines[lineIndex].emplace_back(addedNode);

	return addedNode;
}
