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

protected:
	static float nodeMinWidth;
	static float nodeMinHeight;

protected:
	ImVec2 m_drawLeftTop = ImVec2(0.f, 0.f);
	ImVec2 m_drawNodeHeaderPos = ImVec2(0.f, 0.f);
	ImVec2 m_drawNodeFieldPos = ImVec2(0.f, 0.f);
	ImVec2 m_nodeHeaderSize = ImVec2(0.f, 0.f);
	ImVec2 m_nodeFieldSize = ImVec2(0.f, 0.f);
	ImVec2 m_totalSize = ImVec2(0.f, 0.f);
	ImVec2 m_offsetFromLeftTop = ImVec2(0.f, 0.f);

public:
	const ImVec2& GetDrawNodeHeaderPos() { return m_drawNodeHeaderPos; }
	const ImVec2& GetDrawNodeFieldPos() { return m_drawNodeFieldPos; }
	const ImVec2& GetDrawNodeHeaderSize() { return m_nodeHeaderSize; }
	const ImVec2& GetDrawNodeFieldSize() { return m_nodeFieldSize; }

protected:
	bool m_isDragging = false;

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual void AdjustPosition() override;
	virtual ImVec2 GetInternalNodeSize() = 0;

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
	virtual void OnDragging(MouseEventArgs& args) override;
};

