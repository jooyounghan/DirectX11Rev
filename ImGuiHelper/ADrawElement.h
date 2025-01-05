#pragma once
#include "AInteractable.h"

#include <string>

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
	bool m_isFocused = false;

public:
	std::function<void(ADrawElement*)> OnFocused = [&](ADrawElement*) {};

public:
	void Draw(ImDrawList* drawListIn);
	const ImVec2& GetReferencedOrigin() { return m_referencedOrigin; }

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager) ;
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);
	virtual void SetFocused(const bool& isFocused);
	virtual void OnMouseClicked(MouseClickEventArgs& args) override;
	virtual void OnMouseDown(MouseClickEventArgs& args) override;
	virtual void OnMouseEnter(MouseEventArgs& args) override;
	virtual void OnMouseLeave(MouseEventArgs& args) override;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) = 0;
};

