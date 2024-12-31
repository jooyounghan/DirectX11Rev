#pragma once
#include "ADrawElement.h"

class ANode : public ADrawElement
{
public:
	ANode(const ImVec2& leftTop, const ImVec2& size, const ImVec2& referencedOrigin);

protected:
	ImVec2 m_rightBottom;
	ImVec2 m_size;

protected:
	const ImVec2& m_referencedOrigin;

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual void AdjustPosition() override;

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
};

