#include "APort.h"
#include "Node.h"
#include "DrawElementColor.h"

APort::APort(
    Node* parentNode, const bool& isLeft,
    size_t indexCount, size_t portIndex,
    const float& radius, const ImVec2& referencedOrigin,
    const ImU32& baseColor, const ImU32& hoveringColor
)
    : ADrawElement(referencedOrigin, baseColor), m_radius(radius), m_parentNode(parentNode),
    m_isLeft(isLeft), m_indexCount(indexCount), m_portIndex(portIndex), m_hilightedColor(hoveringColor)
{
}

bool APort::IsPointIn(const float& pointX, const float& pointY) const
{
    return m_radius >= sqrtf((m_drawCenter.x - pointX) * (m_drawCenter.x - pointX) +
        (m_drawCenter.y - pointY) * (m_drawCenter.y - pointY));
}

void APort::DrawImpl(ImDrawList* drawListIn)
{
    drawListIn->AddCircleFilled(m_drawCenter, m_radius, m_isFocused ? m_hilightedColor : m_baseColor);
    drawListIn->AddCircle(m_drawCenter, m_radius, borderLineColor, NULL, 2.f);
    DrawPortConnection(drawListIn);
}

void APort::AdjustPosition()
{
    const ImVec2 nodeFieldSize = m_parentNode->GetDrawNodeFieldSize();
    const ImVec2 nodeDrawFieldLeftTop = m_parentNode->GetDrawNodeFieldPos();
    const ImVec2 nodeDrawFieldRightBottom = ImVec2(nodeDrawFieldLeftTop.x + nodeFieldSize.x, nodeDrawFieldLeftTop.y + nodeFieldSize.y);

    m_drawCenter.x = m_isLeft ? nodeDrawFieldLeftTop.x : nodeDrawFieldRightBottom.x;
    m_drawCenter.y = nodeFieldSize.y * ((m_portIndex + 1) / static_cast<float>(m_indexCount + 1)) + nodeDrawFieldLeftTop.y;
}

void APort::DrawBezierForConnection(ImDrawList* drawListIn, const ImVec2& start, const ImVec2& end)
{
    ImVec2 control1 = ImVec2(start.x + (end.x - start.x) * 0.75f, start.y);
    ImVec2 control2 = ImVec2(end.x - (end.x - start.x) * 0.75f, end.y);

    drawListIn->AddBezierCubic(start, control1, control2, end, m_baseColor, 5.f);
}

void APort::OnBeginDrag()
{
    ADrawElement::OnBeginDrag();
    m_isConnecting = true;
    m_mousePositionDuringConnect = m_drawCenter;
}

void APort::OnDragging(MouseEventArgs& args)
{
    ADrawElement::OnDragging(args);
    m_mousePositionDuringConnect = ImVec2(args.m_mousePosX, args.m_mousePosY);
}

void APort::OnEndDrag()
{
    ADrawElement::OnEndDrag();
    m_isConnecting = false;
}
