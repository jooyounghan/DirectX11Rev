#pragma once
#include "ADrawElement.h"
#include <string>

class Node : public ADrawElement
{
public:
	Node(
		const std::string& nodeName,
		const ImVec2& leftTop, const ImVec2& size, 
		const ImVec2& referencedOrigin,
		const ImU32& baseColor, const ImU32& hoveringColor,
		const ImU32& borderColor, const ImU32& hilightedBoderColor
	);
	~Node() override = default;

protected:
	std::string m_nodeName;
	ImVec2 m_leftTop;
	ImVec2 m_rightBottom;
	ImVec2 m_size;

public:
	inline const ImVec2& GetLeftTop() { return m_leftTop; }
	inline const ImVec2& GetRightBottom() { return m_rightBottom; }

protected:
	ImVec2 m_drawLeftTop;
	ImVec2 m_drawRightBottom;

protected:
	bool m_isDragging = false;

public:
	std::function<void(Node*)> OnMouseClickedHandler = [&](Node* node) {};

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual void AdjustPosition() override;

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
	virtual void OnDragging(MouseEventArgs& args) override;
};

