#pragma once
#include "IGuiControl.h"
#include "ADrawElement.h"

#include <list>
#include <memory>

class NodeCanvas : public IGuiControl
{
public:
	NodeCanvas();

public:
	virtual void RenderControl() override;

protected:
	ImVec2 LeftTopPositon;
	ImVec2 CanvasSize;
	ImVec2 RightBottomPosition;

protected:
	ImVec2 ScrollingPosition;
	
protected:
	DrawElementDragDelegation OnNodeElementDrag;
	DrawElementClickedDelegation OnNodeElementClicked;

protected:
	std::list<std::unique_ptr<ADrawElement>> DrawElements;

private:
	void DrawGrid(const float& GridStepSize);

protected:
	ADrawElement* SelectedNodeElement;

private:
	void DragNode(ADrawElement* NodeDrawElement, const ImVec2& DeltaPos);
	void ClickNode(ADrawElement* NodeDrawElement);
};

