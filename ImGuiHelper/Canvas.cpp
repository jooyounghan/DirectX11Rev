#include "Canvas.h"

using namespace std;
using namespace ImGui;

Canvas::Canvas()
{
}

void Canvas::RenderControlImpl()
{
    ImGuiIO& io = GetIO();
    
    m_leftTop.x = m_left;
    m_leftTop.y = m_top;
    const ImVec2 availRegionSize = GetContentRegionAvail();
    m_canvasSize.x = availRegionSize.x < 10.f ? 10.f : availRegionSize.x;
    m_canvasSize.y = availRegionSize.y < 10.f ? 10.f : availRegionSize.y;
    m_rightBottom.x = m_left + m_canvasSize.x;
    m_rightBottom.y = m_top + m_canvasSize.y;

    RenderGridStep(32.f);

    ImGui::SetNextItemAllowOverlap();
    InvisibleButton("CanvasInvisibleButton", m_canvasSize, ImGuiButtonFlags_::ImGuiButtonFlags_MouseButtonMiddle);
    if (IsItemActive() && IsMouseDragging(ImGuiMouseButton_Middle))
    {
        m_scrollingPosition.x += io.MouseDelta.x;
        m_scrollingPosition.y += io.MouseDelta.y;
    }


    ImDrawList* drawList = GetWindowDrawList();
    m_originPosition = ImVec2(m_left + m_scrollingPosition.x, m_top + m_scrollingPosition.y);
    const ImVec2 canvasMousePos(io.MousePos.x - m_originPosition.x, io.MousePos.y - m_originPosition.y);

}

void Canvas::RenderGridStep(const float& gridStepSize)
{
    ImDrawList* DrawList = GetWindowDrawList();

    DrawList->AddRectFilled(m_leftTop, m_rightBottom, IM_COL32(50, 50, 50, 255));
    DrawList->AddRect(m_leftTop, m_rightBottom, IM_COL32(255, 255, 255, 255));

    DrawList->PushClipRect(m_leftTop, m_rightBottom, true);

    for (float x = fmodf(m_scrollingPosition.x, gridStepSize); x < m_canvasSize.x; x += gridStepSize)
    {
        DrawList->AddLine(ImVec2(m_left + x, m_top), ImVec2(m_left + x, m_rightBottom.y), IM_COL32(200, 200, 200, 40));
    }
    for (float y = fmodf(m_scrollingPosition.y, gridStepSize); y < m_canvasSize.y; y += gridStepSize)
    {
        DrawList->AddLine(ImVec2(m_left, m_top + y), ImVec2(m_rightBottom.x, m_top + y), IM_COL32(200, 200, 200, 40));
    }
}

bool Canvas::IsPointIn(const float& pointX, const float& pointY) const
{
	return m_leftTop.x <= pointX && pointX <= m_rightBottom.x && m_leftTop.y <= pointY && pointY <= m_rightBottom.y;
}
