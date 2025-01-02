#include "APort.h"
#include "Node.h"

APort::APort(
    Node* parentNode, const bool& isLeft,
    size_t indexCount, size_t portIndex,
    const float& radius, const ImVec2& referencedOrigin,
    const ImU32& baseColor, const ImU32& hoveringColor,
    const ImU32& borderColor, const ImU32& hilightedBoderColor
)
    : ADrawElement(
        referencedOrigin,
        baseColor, hoveringColor,
        borderColor, hilightedBoderColor
    ), m_radius(radius), m_parentNode(parentNode),
    m_isLeft(isLeft), m_indexCount(indexCount), m_portIndex(portIndex)
{
    SetZIndex(parentNode->GetZIndex() + 1);
}

bool APort::IsPointIn(const float& pointX, const float& pointY) const
{
    return m_radius >= sqrtf((m_drawCenter.x - pointX) * (m_drawCenter.x - pointX) +
        (m_drawCenter.y - pointY) * (m_drawCenter.y - pointY));
}

void APort::DrawImpl(ImDrawList* drawListIn)
{
    drawListIn->AddCircleFilled(m_drawCenter, m_radius, m_selectedColor);
    drawListIn->AddCircle(m_drawCenter, m_radius, m_selectedBorderColor, NULL, 2.f);

    DrawPortConnection(drawListIn);
}

void APort::AdjustPosition()
{
    const ImVec2 nodeLeftTop = m_parentNode->GetLeftTop();
    const ImVec2 nodeRightBottom = m_parentNode->GetRightBottom();

    m_center.x = m_isLeft ? nodeLeftTop.x : nodeRightBottom.x;
    m_center.y = (nodeRightBottom.y - nodeLeftTop.y)
        * ((m_portIndex + 1) / static_cast<float>(m_indexCount + 1)) + nodeLeftTop.y;
    m_drawCenter = ImVec2(m_center.x + m_referencedOrigin.x, m_center.y + m_referencedOrigin.y);
}

void APort::DrawBezierForConnection(ImDrawList* drawListIn, const ImVec2& start, const ImVec2& end)
{
    ImVec2 control1 = ImVec2(start.x + (end.x - start.x) * 0.75f, start.y);
    ImVec2 control2 = ImVec2(end.x - (end.x - start.x) * 0.75f, end.y);

    drawListIn->AddBezierCubic(start, control1, control2, end, m_borderColor, 5.f);
}


