#pragma once
#include "AInteractable.h"

class DrawElementManager;

class ADrawElement : public AInteractable
{
public:
	ADrawElement(const ImVec2& referencedOrigin, const ImU32& baseColor);
	~ADrawElement() override = default;

protected:
	const ImVec2& m_referencedOrigin;
	const ImU32 m_baseColor;
	ImU32 m_selectedBorderFill;

protected:
	bool m_isHilghted = false;

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager) ;
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);

public:
	void SetHilighted(const bool& isHilighted);

public:
	void Draw(ImDrawList* drawListIn);

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) = 0;

public:
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
	virtual void OnMouseDown(MouseClickEventArgs& args) override;

public:
	virtual void OnMouseEnter(MouseEventArgs& args) override;
	virtual void OnMouseLeave(MouseEventArgs& args) override;


};

