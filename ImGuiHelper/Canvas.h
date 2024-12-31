#pragma once
#include "AUserControl.h"
#include "ImGuiControlManager.h"
#include <vector>

class ADrawElement;

class Canvas : public AUserControl, public ImGuiInteractionManager
{
public:
	Canvas();
	~Canvas() override;

protected:
	ImVec2 m_rightBottom;
	ImVec2 m_canvasSize;

protected:
	ImVec2 m_originPosition = ImVec2(0.f, 0.f);
	ImVec2 m_scrollingPosition = ImVec2(0.f, 0.f);

public:
	void AddNode(const ImVec2& leftTop, const ImVec2& size);

private:
	std::vector<ADrawElement*> m_drawElements;

protected:
	virtual void RenderControlImpl() override;
	virtual void AdjustPosition() override;

private:
	void RenderGridStep(const float& gridStepSize);

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;
};

