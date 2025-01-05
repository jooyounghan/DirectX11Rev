#include "FlowOutputPort.h"
#include "FlowNode.h"
#include "NodeConstant.h"

void FlowOutputPort::Draw(ImDrawList* drawListIn)
{
	APort::Draw(drawListIn);

	const ImVec2 leftTopPoint = ImVec2(m_drawCenter.x + (m_radius + portInternalMargin), m_drawCenter.y + m_radius);
	const ImVec2 leftBottomPoint = ImVec2(leftTopPoint.x, leftTopPoint.y - 2.f * m_radius);
	const ImVec2 RightPoint = ImVec2(leftBottomPoint.x + 2.f * m_radius, m_drawCenter.y);

	drawListIn->AddTriangle(leftTopPoint, leftBottomPoint, RightPoint, borderLineColor, borederThickness);
	drawListIn->AddTriangleFilled(leftTopPoint, leftBottomPoint, RightPoint, flowTypeColor);
}

void FlowOutputPort::DrawPortConnection(ImDrawList* drawListIn)
{
	if (m_isConnecting)
	{
		DrawBezierForConnection(drawListIn, m_drawCenter, m_mousePositionDuringConnect);
	}
}

void FlowOutputPort::OnMouseUp(MouseClickEventArgs& args)
{
	FlowPort::OnMouseUp(args);

	if (m_connectingInputPort != nullptr)
	{
		m_connectingOutputPort = this;
		m_connectingInputPort->OnMouseUp(args);
		m_connectingOutputPort = nullptr;
	}
}

void FlowOutputPort::OnBeginDrag()
{
	FlowPort::OnBeginDrag();
	m_connectingOutputPort = this;
}

void FlowOutputPort::OnEndDrag()
{
	FlowPort::OnEndDrag();
	m_connectingOutputPort = nullptr;
}
