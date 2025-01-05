#pragma once
#include "ADrawElement.h"

class Node;

class APort : public ADrawElement
{
public:
	APort(
		Node* parentNode, const bool& isLeft,
		size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin,
		const ImU32& baseColor, const ImU32& hoveringColor
	);
	~APort() override = default;

protected:
	Node* m_parentNode;
	const bool m_isLeft;
	const size_t m_indexCount;
	const size_t m_portIndex;

protected:
	const ImU32 m_hilightedColor;

protected:
	ImVec2 m_drawCenter = ImVec2(0.f, 0.f);
	ImVec2 m_mousePositionDuringConnect = ImVec2(0.f, 0.f);
	float m_radius;
	bool m_isConnecting = false;

public:
	inline const ImVec2& GetDrawCenter() { return m_drawCenter; }
	inline const float& GetRadius() { return m_radius; }
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;
	virtual void Draw(ImDrawList* drawListIn) override;

protected:
	void DrawBezierForConnection(ImDrawList* drawListIn, const ImVec2& start, const ImVec2& end);

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) = 0;

public:
	virtual void OnBeginDrag() override;
	virtual void OnDragging(MouseEventArgs& args) override;
	virtual void OnEndDrag() override;
};

