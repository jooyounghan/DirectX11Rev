#pragma once
#include "IGuiControl.h"
#include "ADrawElement.h"
#include "StaticAssertHelper.h"
#include "NodeElement.h"
#include "AOutputPort.h"
#include "InputPort.h"

#include <list>
#include <memory>

class ANodeCanvas : public IGuiControl
{
public:
	ANodeCanvas(const ImVec2& CanvasSizeIn);
	virtual ~ANodeCanvas();

public:
	virtual void RenderControl() override;

protected:
	ImVec2 LeftTopPositon;
	ImVec2 RightBottomPosition;

protected:
	ImVec2 CanvasSize;
	MakeGetter(CanvasSize);

protected:
	ImVec2 OriginPosition;

protected:
	ImVec2 ScrollingPosition;
	
protected:
	ClickedElementHandler			OnNodeClicked;

protected:
	ClickedElementHandler			OnPortClicked;
	MouseEnterElementHandler			OnPortEnter;
	MouseLeaveElementHandler			OnPortLeave;

protected:
	std::list<std::unique_ptr<ADrawElement>> DrawElements;

protected:
	void DrawCanvasRectangle(const float& GridStepSize);

public:
	template<typename T, typename ...Args>
	T* AddNodeElement(Args... args);

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
T* ANodeCanvas::AddNodeElement(Args ...args)
{
	static_assert(std::is_base_of<NodeElement, T>::value, DerivedCondition(NodeElement));

	DrawElements.emplace_back(make_unique<T>(
		args...
	));


	NodeElement* NodeElementCached = ((NodeElement*)DrawElements.back().get());
	NodeElementCached->ClickedEvent += OnNodeClicked;

	for (auto& OutputPortCached : NodeElementCached->GetOutputPorts())
	{
		OutputPortCached->ClickedEvent += OnPortClicked;
	}

	for (auto& InputPortCached : NodeElementCached->GetInputPorts())
	{
		InputPortCached->MouseEnterEvent += OnPortEnter;
		InputPortCached->MouseLeaveEvent += OnPortLeave;
	}

	return (T*)NodeElementCached;
}
