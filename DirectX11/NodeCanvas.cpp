#include "NodeCanvas.h"

#include "RectangleDrawElement.h"

using namespace std;
using namespace ImGui;

NodeCanvas::NodeCanvas()
{
    LeftTopPositon = GetCursorScreenPos();
    CanvasSize = GetContentRegionAvail();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);


    OnNodeElementDrag = bind(&NodeCanvas::DragNode, this, placeholders::_1, placeholders::_2);
    OnNodeElementClicked = bind(&NodeCanvas::ClickNode, this, placeholders::_1);

    for (int i = 0; i < 2; ++i)
    {
        DrawElements.emplace_back(make_unique<RectangleDrawElement>(
            RectangleDrawElement(
                ImVec2(LeftTopPositon.x + CanvasSize.x / 2.f, LeftTopPositon.y + CanvasSize.y / 2.f),
                ImVec2(100.f, 100.f),
                IM_COL32(180, 100, 100, 255),
                IM_COL32(255, 100, 100, 255)
            )
        ));
        DrawElements.back()->DrawElementClickedEvent += OnNodeElementClicked;
        DrawElements.back()->DrawElementDragEvent += OnNodeElementDrag;
    }
}

void NodeCanvas::RenderControl()
{
    static ImVector<ImVec2> points;

    static bool adding_line = false;

    LeftTopPositon = GetCursorScreenPos();
    CanvasSize = GetContentRegionAvail();
    if (CanvasSize.x < 50.0f) CanvasSize.x = 50.0f;
    if (CanvasSize.y < 50.0f) CanvasSize.y = 50.0f;
    RightBottomPosition = ImVec2(LeftTopPositon.x + CanvasSize.x, LeftTopPositon.y + CanvasSize.y);

    // Draw border and background color
    ImGuiIO& io = GetIO();
    ImDrawList* draw_list = GetWindowDrawList();
    draw_list->AddRectFilled(LeftTopPositon, RightBottomPosition, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(LeftTopPositon, RightBottomPosition, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    InvisibleButton("canvas", CanvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = IsItemHovered(); // Hovered
    const bool is_active = IsItemActive();   // Held
    const ImVec2 origin(LeftTopPositon.x + ScrollingPosition.x, LeftTopPositon.y + ScrollingPosition.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Add first and second point
    if (is_hovered && !adding_line && IsMouseClicked(ImGuiMouseButton_Left))
    {
        points.push_back(mouse_pos_in_canvas);
        points.push_back(mouse_pos_in_canvas);
        adding_line = true;
    }
    if (adding_line)
    {
        points.back() = mouse_pos_in_canvas;
        if (!IsMouseDown(ImGuiMouseButton_Left))
            adding_line = false;
    }

    if (is_active && IsMouseDragging(ImGuiMouseButton_Right))
    {
        ScrollingPosition.x += io.MouseDelta.x;
        ScrollingPosition.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = GetMouseDragDelta(ImGuiMouseButton_Right);
    if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (BeginPopup("context"))
    {
        if (adding_line)
            points.resize(points.size() - 2);
        adding_line = false;
        if (MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
        if (MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
        EndPopup();
    }

    DrawGrid(32.f);

    for (unique_ptr<ADrawElement>& DrawElement : DrawElements)
    {
        DrawElement->AddToDrawList(origin, draw_list);
    }
}

void NodeCanvas::DrawGrid(const float& GridStepSize)
{
    ImDrawList* draw_list = GetWindowDrawList();
 
    draw_list->PushClipRect(LeftTopPositon, RightBottomPosition, true);

    for (float x = fmodf(ScrollingPosition.x, GridStepSize); x < CanvasSize.x; x += GridStepSize)
    {
        draw_list->AddLine(ImVec2(LeftTopPositon.x + x, LeftTopPositon.y), ImVec2(LeftTopPositon.x + x, RightBottomPosition.y), IM_COL32(200, 200, 200, 40));
    }
    for (float y = fmodf(ScrollingPosition.y, GridStepSize); y < CanvasSize.y; y += GridStepSize)
    {
        draw_list->AddLine(ImVec2(LeftTopPositon.x, LeftTopPositon.y + y), ImVec2(RightBottomPosition.x, LeftTopPositon.y + y), IM_COL32(200, 200, 200, 40));
    }
    draw_list->PopClipRect();
}

void NodeCanvas::DragNode(ADrawElement* NodeDrawElement, const ImVec2& MousePos)
{
    ImGuiIO& io = GetIO();
    const ImVec2 origin(LeftTopPositon.x + ScrollingPosition.x, LeftTopPositon.y + ScrollingPosition.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    NodeDrawElement->SetPosition(mouse_pos_in_canvas);
}

void NodeCanvas::ClickNode(ADrawElement* NodeDrawElement)
{
    if (SelectedNodeElement != nullptr)
    {
        SelectedNodeElement->SetFocus(false);
    }
    if (NodeDrawElement != nullptr)
    {
        NodeDrawElement->SetFocus(true);
    }
    SelectedNodeElement = NodeDrawElement;
}
