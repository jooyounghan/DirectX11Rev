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
		const ImU32& baseColor, const ImU32& hoveringColor,
		const ImU32& borderColor, const ImU32& hilightedBoderColor
	);
	~APort() override = default;

protected:
	Node* m_parentNode;
	const bool m_isLeft;
	const size_t m_indexCount;
	const size_t m_portIndex;

protected:
	ImVec2 m_center = ImVec2(0.f, 0.f);
	ImVec2 m_drawCenter = ImVec2(0.f, 0.f);
	float m_radius;
	 
public:
	inline const ImVec2& GetCenter() { return m_center; }
	inline const ImVec2& GetDrawCenter() { return m_drawCenter; }

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual void AdjustPosition() override;
	void DrawBezierForConnection(ImDrawList* drawListIn, const ImVec2& start, const ImVec2& end);

protected:
	virtual void DrawPortConnection(ImDrawList* drawListIn) = 0;
};

