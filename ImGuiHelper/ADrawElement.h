#pragma once
#include "AInteractable.h"

class ADrawElement : public AInteractable
{
public:
	ADrawElement(
		const ImVec2& referencedOrigin,
		const ImU32& baseColor, const ImU32& hoveringColor,
		const ImU32& borderColor, const ImU32& hilightedBoderColor
	);
	~ADrawElement() override = default;

protected:
	const ImVec2& m_referencedOrigin;
	ImU32 m_baseColor;
	ImU32 m_hoveringColor;
	ImU32 m_borderColor;
	ImU32 m_hilightedBorderColor;

protected:
	ImU32* m_selectedColor;
	ImU32* m_selectedBorderColor;

protected:
	bool m_isHilghted = false;

public:
	void SetHilighted(const bool& isHilighted);

public:
	void Draw(ImDrawList* drawListIn);

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) = 0;

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;

public:
	virtual void OnMouseEnter(MouseEventArgs& args) override;
	virtual void OnMouseLeave(MouseEventArgs& args) override;


};

