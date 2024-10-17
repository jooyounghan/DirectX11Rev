#include "NodeCanvas.h"

#include "VariableInputNode.h"
#include "VariableOutputNode.h"
#include "VariablePredicateNode.h"
#include "FlowStartNode.h"
#include "FlowMidNode.h"
#include "FlowEndNode.h"

#include <Windows.h>

using namespace std;
using namespace ImGui;

NodeCanvas::NodeCanvas()
{
    LeftTopPositon = GetCursorScreenPos();
    CanvasSize = GetContentRegionAvail();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);

    OnNodeClicked = bind(&NodeCanvas::SelectNode, this, placeholders::_1);

    OnPortClicked = bind(&NodeCanvas::SelectPort, this, placeholders::_1);
    OnPortEnter = bind(&NodeCanvas::MouseEnterPort, this, placeholders::_1);
    OnPortLeave = bind(&NodeCanvas::MouseLeavePort, this, placeholders::_1);


    AddNodeElement<VariableInputNode>(
        ImVec2(100.f + LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );

    AddNodeElement<VariableInputNode>(
        ImVec2(LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );


    AddNodeElement<VariableInputNode>(
        ImVec2(200.f + LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );

    AddNodeElement<VariableOutputNode>(
        ImVec2(250.f + LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );

    AddNodeElement<VariablePredicateNode>(
        ImVec2(400.f + LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f), 3
    );

    AddNodeElement<FlowStartNode>(
        ImVec2(LeftTopPositon.x + CanvasSize.x / 2.f, 200.f + LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );

    AddNodeElement<FlowMidNode>(
        ImVec2(100.f + LeftTopPositon.x + CanvasSize.x / 2.f, 200.f + LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f), 1
    );
    AddNodeElement<FlowMidNode>(
        ImVec2(250.f + LeftTopPositon.x + CanvasSize.x / 2.f, 200.f + LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f), 2
    );

    AddNodeElement<FlowMidNode>(
        ImVec2(400.f + LeftTopPositon.x + CanvasSize.x / 2.f, 200.f + LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f), 3
    );

    AddNodeElement<FlowEndNode>(
        ImVec2(550.f + LeftTopPositon.x + CanvasSize.x / 2.f, 200.f + LeftTopPositon.y + CanvasSize.y / 2.f),
        ImVec2(100.f, 100.f)
    );
}

void NodeCanvas::RenderControl()
{
    ImGuiIO& io = GetIO();

    LeftTopPositon = GetCursorScreenPos();
    CanvasSize = GetContentRegionAvail();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);

    DrawCanvasRectangle(32.f);

    InvisibleButton("NodeCanvas", CanvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    if (IsItemActive() && IsMouseDragging(ImGuiMouseButton_Right))
    {
        ScrollingPosition.x += io.MouseDelta.x;
        ScrollingPosition.y += io.MouseDelta.y;
    }

    ImDrawList* DrawList = GetWindowDrawList();
    const ImVec2 Origin(LeftTopPositon.x + ScrollingPosition.x, LeftTopPositon.y + ScrollingPosition.y);
    const ImVec2 CanvasMousePos(io.MousePos.x - Origin.x, io.MousePos.y - Origin.y);

    ResetStatus();

    for (unique_ptr<ADrawElement>& DrawElement : DrawElements)
    {
        DrawElement->AddToDrawList(Origin, DrawList);
    }

    if (!bIsNodeSelectedOnTick && IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        ResetSelectedNode();
    }

    if (!bIsPortSelectedOnTick && IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        ResetSelectedOutputPort();
    }

    if (IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        if (SelectedOutputPort != nullptr)
        {
            SelectedOutputPort->Connect(TargetPort);
        }
        ResetSelectedOutputPort();
    }

    if (SelectedOutputPort== nullptr && SelectedNodeElement != nullptr && IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        SelectedNodeElement->SetPosition(CanvasMousePos);
    }
 
    ShowContextMenu();
}

/* Node Canvas가 구체화 된 곳에 구현 필요 */
void NodeCanvas::ShowContextMenu()
{    
    ImVec2 DragDelta = GetMouseDragDelta(ImGuiMouseButton_Right);
    if (DragDelta.x == 0.0f && DragDelta.y == 0.0f)
    {
        OpenPopupOnItemClick("CavnasContextMenu", ImGuiPopupFlags_MouseButtonRight);
    }
    if (BeginPopup("CavnasContextMenu"))
    {
        if (MenuItem("Add Node", NULL, false, true)) {  }
        if (MenuItem("Add", NULL, false, true)) {  }
        EndPopup();
    }

}

void NodeCanvas::DrawCanvasRectangle(const float& GridStepSize)
{
    ImDrawList* DrawList = GetWindowDrawList();

    DrawList->AddRectFilled(LeftTopPositon, RightBottomPosition, IM_COL32(50, 50, 50, 255));
    DrawList->AddRect(LeftTopPositon, RightBottomPosition, IM_COL32(255, 255, 255, 255));

    DrawList->PushClipRect(LeftTopPositon, RightBottomPosition, true);

    for (float x = fmodf(ScrollingPosition.x, GridStepSize); x < CanvasSize.x; x += GridStepSize)
    {
        DrawList->AddLine(ImVec2(LeftTopPositon.x + x, LeftTopPositon.y), ImVec2(LeftTopPositon.x + x, RightBottomPosition.y), IM_COL32(200, 200, 200, 40));
    }
    for (float y = fmodf(ScrollingPosition.y, GridStepSize); y < CanvasSize.y; y += GridStepSize)
    {
        DrawList->AddLine(ImVec2(LeftTopPositon.x, LeftTopPositon.y + y), ImVec2(RightBottomPosition.x, LeftTopPositon.y + y), IM_COL32(200, 200, 200, 40));
    }
}

void NodeCanvas::ResetStatus()
{
    bIsNodeSelectedOnTick = false;
    bIsPortSelectedOnTick = false;
}

void NodeCanvas::SelectNode(ADrawElement* DrawElement)
{
    bIsNodeSelectedOnTick = true;
    ResetSelectedNode();
    SelectedNodeElement = (NodeElement*)DrawElement;
    SelectedNodeElement->SetFocus(true);
}

void NodeCanvas::ResetSelectedNode()
{
    if (SelectedNodeElement != nullptr)
    {
        SelectedNodeElement->SetFocus(false);
    }
    SelectedNodeElement = nullptr;
}

void NodeCanvas::MouseEnterPort(ADrawElement* DrawElement)
{
    if (SelectedOutputPort != nullptr)
    {
        TargetPort = (InputPort*)(DrawElement);
    }
}

void NodeCanvas::MouseLeavePort(ADrawElement* DrawElement)
{
    TargetPort = nullptr;
}

void NodeCanvas::SelectPort(ADrawElement* DrawElement)
{
    bIsPortSelectedOnTick = true;
    ResetSelectedOutputPort();
    SelectedOutputPort = (AOutputPort*)DrawElement;
    SelectedOutputPort->SetIsConnecting(true);
}

void NodeCanvas::ResetSelectedOutputPort()
{
    if (SelectedOutputPort != nullptr)
    {
        SelectedOutputPort->SetIsConnecting(false);
        SelectedOutputPort = nullptr;
    }
}

