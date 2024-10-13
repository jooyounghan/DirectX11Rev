#pragma once
#include "IGuiControl.h"
#include "ADrawElement.h"

#include <list>
#include <memory>

// Node로 변경 필요!
class RectangleDrawElement;

class OutputPort;
class InputPort;

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
	ClickedElementDelegation			OnElementClicked;

protected:
	ClickedElementDelegation			OnPortClicked;
	MouseEnterElementDelegation			OnPortEnter;
	MouseLeaveElementDelegation			OnPortLeave;

protected:
	std::list<std::unique_ptr<ADrawElement>> DrawElements;

private:
	virtual void ShowContextMenu();
	void DrawCanvasRectangle(const float& GridStepSize);

protected:
	RectangleDrawElement* SelectedNodeElement = nullptr;

private:
	void ResetStatus();

private:
	bool bIsNodeSelectedOnTick = false;
	void SelectNode(ADrawElement* DrawElement);

private:
	void ResetSelectedNode();

protected:
	OutputPort* SelectedOutputPort = nullptr;
	InputPort* TargetPort = nullptr;

protected:
	bool bIsPortSelectedOnTick = false;
	void SelectPort(ADrawElement* DrawElement);

private:
	void ResetSelectedOutputPort();

private:
	void MouseEnterPort(ADrawElement* DrawElement);
	void MouseLeavePort(ADrawElement* DrawElement);

};

