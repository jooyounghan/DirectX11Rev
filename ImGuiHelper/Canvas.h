#pragma once
#include "AUserControl.h"

class Canvas : public AUserControl
{
public:
	Canvas();

protected:
	ImVec2 m_leftTop;
	ImVec2 m_rightBottom;
	ImVec2 m_canvasSize;


protected:
	ImVec2 m_originPosition;
	ImVec2 m_scrollingPosition;

private:
	virtual void RenderControlImpl() override;
	void RenderGridStep(const float& gridStepSize);

public:
	virtual bool IsPointIn(const float& pointX, const float& pointY) const override;
	virtual void OnMouseClicked(MouseClickEventArgs& args) override 
	{
		bool test = true;
		args.m_isHandled = true;
	};
};

