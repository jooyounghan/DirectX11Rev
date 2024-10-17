#pragma once
#include "IGuiControl.h"
#include "ADrawElement.h"
#include "StaticAssertHelper.h"
#include "NodeElement.h"
#include "AOutputPort.h"
#include "InputPort.h"

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
	ClickedElementDelegation			OnNodeClicked;

protected:
	ClickedElementDelegation			OnPortClicked;
	MouseEnterElementDelegation			OnPortEnter;
	MouseLeaveElementDelegation			OnPortLeave;

protected:
	std::list<std::unique_ptr<ADrawElement>> DrawElements;

private:
	virtual void ShowContextMenu();
	void DrawCanvasRectangle(const float& GridStepSize);

public:
	template<typename T, typename ...Args>
	void AddNodeElement(Args... args);

protected:
	NodeElement* SelectedNodeElement = nullptr;

private:
	void ResetStatus();

private:
	bool bIsNodeSelectedOnTick = false;
	void SelectNode(ADrawElement* DrawElement);

private:
	void ResetSelectedNode();

protected:
	AOutputPort* SelectedOutputPort = nullptr;
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

template<typename T, typename ...Args>
void NodeCanvas::AddNodeElement(Args ...args)
{
	static_assert(std::is_base_of<NodeElement, T>::value, DerivedCondition(NodeElement));

	DrawElements.emplace_back(make_unique<T>(
		args...
	));

	DrawElements.back()->ClickedEvent += OnNodeClicked;
	NodeElement* NodeElementCached = ((NodeElement*)DrawElements.back().get());

	for (auto& OutputPortCached : NodeElementCached->GetOutputPorts())
	{
		OutputPortCached->ClickedEvent += OnPortClicked;
	}

	for (auto& InputPortCached : NodeElementCached->GetInputPorts())
	{
		InputPortCached->MouseEnterEvent += OnPortEnter;
		InputPortCached->MouseLeaveEvent += OnPortLeave;
	}
}
