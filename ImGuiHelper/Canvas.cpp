#include "Canvas.h"
#include <math.h>

using namespace std;
using namespace ImGui;

Canvas::Canvas()
{
}

void Canvas::RenderControlImpl()
{
    ImGuiIO& io = GetIO();

    RenderGridStep(32.f);

    ImGui::SetNextItemAllowOverlap();
    InvisibleButton("CanvasInvisibleButton", m_canvasSize, ImGuiButtonFlags_::ImGuiButtonFlags_MouseButtonMiddle);
    if (IsItemActive() && IsMouseDragging(ImGuiMouseButton_Middle))
    {
        m_scrollingPosition.x += io.MouseDelta.x;
        m_scrollingPosition.y += io.MouseDelta.y;
    }

    ImDrawList* drawList = GetWindowDrawList();
    m_originPosition = ImVec2(m_leftTop.x + m_scrollingPosition.x, m_leftTop.y + m_scrollingPosition.y);

    DrawElements(drawList);
    CheckMouseControlEvents();
}

void Canvas::AdjustPosition()
{
    AUserControl::AdjustPosition();
    const ImVec2 availRegionSize = GetContentRegionAvail();
    m_canvasSize.x = availRegionSize.x < 10.f ? 10.f : availRegionSize.x;
    m_canvasSize.y = availRegionSize.y < 10.f ? 10.f : availRegionSize.y;

    m_rightBottom.x = m_leftTop.x + m_canvasSize.x;
    m_rightBottom.y = m_leftTop.y + m_canvasSize.y;
}

void Canvas::RenderGridStep(const float& gridStepSize)
{
    ImDrawList* DrawList = GetWindowDrawList();

    DrawList->AddRectFilled(m_leftTop, m_rightBottom, IM_COL32(50, 50, 50, 255));
    DrawList->AddRect(m_leftTop, m_rightBottom, IM_COL32(255, 255, 255, 255));

    DrawList->PushClipRect(m_leftTop, m_rightBottom, true);

    for (float x = fmodf(m_scrollingPosition.x, gridStepSize); x < m_canvasSize.x; x += gridStepSize)
    {
        DrawList->AddLine(ImVec2(m_leftTop.x + x, m_leftTop.y), ImVec2(m_leftTop.x + x, m_rightBottom.y), IM_COL32(200, 200, 200, 40));
    }
    for (float y = fmodf(m_scrollingPosition.y, gridStepSize); y < m_canvasSize.y; y += gridStepSize)
    {
        DrawList->AddLine(ImVec2(m_leftTop.x, m_leftTop.y + y), ImVec2(m_rightBottom.x, m_leftTop.y + y), IM_COL32(200, 200, 200, 40));
    }
}

bool Canvas::IsPointIn(const float& pointX, const float& pointY) const
{
	return m_leftTop.x <= pointX && pointX <= m_rightBottom.x && m_leftTop.y <= pointY && pointY <= m_rightBottom.y;
}
