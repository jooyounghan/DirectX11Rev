#include "ANodeCanvas.h"

using namespace std;
using namespace ImGui;

ANodeCanvas::ANodeCanvas(const ImVec2& CanvasSizeIn)
    : CanvasSize(CanvasSizeIn)
{
    LeftTopPositon = GetCursorScreenPos();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);

    OnNodeClicked = bind(&ANodeCanvas::SelectNode, this, placeholders::_1);

    OnPortClicked = bind(&ANodeCanvas::SelectPort, this, placeholders::_1);
    OnPortEnter = bind(&ANodeCanvas::MouseEnterPort, this, placeholders::_1);
    OnPortLeave = bind(&ANodeCanvas::MouseLeavePort, this, placeholders::_1);
}

ANodeCanvas::~ANodeCanvas()
{
}

void ANodeCanvas::RenderControl()
{
    ImGuiIO& io = GetIO();

    LeftTopPositon = GetCursorScreenPos();
    CanvasSize = GetContentRegionAvail();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);

    DrawCanvasRectangle(32.f);

    ImGui::SetNextItemAllowOverlap();
    InvisibleButton("NodeCanvas", CanvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    if (IsItemActive() && IsMouseDragging(ImGuiMouseButton_Right))
    {
        ScrollingPosition.x += io.MouseDelta.x;
        ScrollingPosition.y += io.MouseDelta.y;
    }

    ImDrawList* DrawList = GetWindowDrawList();
    const ImVec2 Origin(LeftTopPositon.x + ScrollingPosition.x, LeftTopPositon.y + ScrollingPosition.y);
    const ImVec2 CanvasMousePos(io.MousePos.x - Origin.x, io.MousePos.y - Origin.y);

    ShowContextMenu(Origin);

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
 
}

void ANodeCanvas::ShowContextMenu(const ImVec2& OriginPosition)
{
    ImVec2 DragDelta = GetMouseDragDelta(ImGuiMouseButton_Right);
    if (DragDelta.x == 0.0f && DragDelta.y == 0.0f)
    {
        OpenPopupOnItemClick(ContextPopUpID, ImGuiPopupFlags_MouseButtonRight);
    }
}

void ANodeCanvas::DrawCanvasRectangle(const float& GridStepSize)
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

void ANodeCanvas::ResetStatus()
{
    bIsNodeSelectedOnTick = false;
    bIsPortSelectedOnTick = false;
}

void ANodeCanvas::SelectNode(ADrawElement* DrawElement)
{
    bIsNodeSelectedOnTick = true;
    ResetSelectedNode();
    SelectedNodeElement = (NodeElement*)DrawElement;
    SelectedNodeElement->SetFocus(true);
}

void ANodeCanvas::ResetSelectedNode()
{
    if (SelectedNodeElement != nullptr)
    {
        SelectedNodeElement->SetFocus(false);
    }
    SelectedNodeElement = nullptr;
}

void ANodeCanvas::MouseEnterPort(ADrawElement* DrawElement)
{
    if (SelectedOutputPort != nullptr)
    {
        TargetPort = (InputPort*)(DrawElement);
    }
}

void ANodeCanvas::MouseLeavePort(ADrawElement* DrawElement)
{
    TargetPort = nullptr;
}

void ANodeCanvas::SelectPort(ADrawElement* DrawElement)
{
    bIsPortSelectedOnTick = true;
    ResetSelectedOutputPort();
    SelectedOutputPort = (AOutputPort*)DrawElement;
    SelectedOutputPort->SetIsConnecting(true);
}

void ANodeCanvas::ResetSelectedOutputPort()
{
    if (SelectedOutputPort != nullptr)
    {
        SelectedOutputPort->SetIsConnecting(false);
        SelectedOutputPort = nullptr;
    }
}

