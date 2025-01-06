#pragma once
#include "ADrawElement.h"
#include <string>

class Node : public ADrawElement
{
public:
	Node(
		const std::string& nodeName, const ImVec2& leftTop, 
		const ImVec2& referencedOrigin, const ImU32& baseColor
	);
	~Node() override = default;

protected:
	std::string m_nodeName;

protected:
	ImVec2 m_leftTop;

public:
	inline const ImVec2& GetLeftTop() { return m_leftTop; }
	inline void SetLeftTop(const ImVec2& leftTop) { m_leftTop = leftTop; }

protected:
	ImVec2 m_drawLeftTop = ImVec2(0.f, 0.f);
	ImVec2 m_offsetFromLeftTop = ImVec2(0.f, 0.f);
	ImVec2 m_headerTextSize = ImVec2(0.f, 0.f);
	ImVec2 m_headerSize = ImVec2(0.f, 0.f);
	ImVec2 m_totalSize = ImVec2(0.f, 0.f);

public:
	ImVec2 GetFieldDrawLeftTopPos() { return ImVec2(m_drawLeftTop.x, m_drawLeftTop.y + m_headerSize.y); }
	ImVec2 GetFieldDrawRightBottomPos() { return ImVec2(m_drawLeftTop.x + m_totalSize.x, m_drawLeftTop.y + m_totalSize.y - m_headerSize.y); }
	ImVec2 GetFieldSize() { return ImVec2(m_totalSize.x, m_totalSize.y - m_headerSize.y); }
	const ImVec2& GetTotalSize() { return m_totalSize; }

protected:
	bool m_isDragging = false;

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;

public:
	static void SetItemCursorWithInternalMargin(const ImVec2& drawLeftTop);
	static ImVec2 GetItemRectWithInternalMargin();
	static float GetDynamicWidthWithoutInternalMargin(const float& referenceWidth);

protected:
	virtual void Draw(ImDrawList* drawListIn) override;
	
protected:
	using ImGuiDrawFunction = std::function<ImVec2(const ImVec2&, ImDrawList*)>;
	std::vector<std::vector<ImGuiDrawFunction>> m_drawsCommands;

protected:
	void AddDrawCommand(const ImGuiDrawFunction& drawCommand);
	void AddDrawCommandSameLine(const ImGuiDrawFunction& drawCommand);
	
public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
	virtual void OnDragging(MouseEventArgs& args) override;
};

