#pragma once
#include "AUserControl.h"
#include "InteractionManager.h"
#include "DrawElementManager.h"

class ADrawElement;

class Canvas : public AUserControl, public InteractionManager, public DrawElementManager
{
public:
	Canvas();
	~Canvas() override = default;

protected:
	ImVec2 m_rightBottom;
	ImVec2 m_canvasSize;

protected:
	ImVec2 m_originPosition = ImVec2(0.f, 0.f);
	ImVec2 m_scrollingPosition = ImVec2(0.f, 0.f);

public:
	inline const ImVec2& GetOriginPosition() { return m_originPosition; }

protected:
	virtual void AddDrawElement(ADrawElement* drawElement);
	virtual void RenderControlImpl() override;
	virtual void AdjustPosition() override;

private:
	void RenderGridStep(const float& gridStepSize);

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;
};

