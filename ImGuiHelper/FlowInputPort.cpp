#include "FlowInputPort.h"
#include "FlowOutputPort.h"
#include "NodeConstant.h"

void FlowInputPort::Draw(ImDrawList* drawListIn)
{
	APort::Draw(drawListIn);

	const ImVec2 leftTopPoint = ImVec2(m_drawCenter.x - (3.f * m_radius + portInternalMargin), m_drawCenter.y + m_radius);
	const ImVec2 leftBottomPoint = ImVec2(leftTopPoint.x, leftTopPoint.y - 2.f * m_radius);
	const ImVec2 RightPoint = ImVec2(leftBottomPoint.x + 2.f * m_radius, m_drawCenter.y);

	drawListIn->AddTriangle(leftTopPoint, leftBottomPoint, RightPoint, borderLineColor, borederThickness);
	drawListIn->AddTriangleFilled(leftTopPoint, leftBottomPoint, RightPoint, flowTypeColor);

}

void FlowInputPort::DrawPortConnection(ImDrawList* drawListIn)
{
	if (m_connectedPort != nullptr || m_isConnecting)
	{
		if (m_connectedPort != nullptr)
		{
			m_mousePositionDuringConnect = m_connectedPort->GetDrawCenter();
		}

		DrawBezierForConnection(drawListIn, m_drawCenter, m_mousePositionDuringConnect);
	}
}

void FlowInputPort::OnMouseUp(MouseClickEventArgs& args)
{
	FlowPort::OnMouseUp(args);

	if (m_connectingOutputPort != nullptr)
	{
		SetConnectedOutputPort((FlowOutputPort*)m_connectingOutputPort);
	}
}

void FlowInputPort::OnBeginDrag()
{
	FlowPort::OnBeginDrag();
	m_connectingInputPort = this;
	m_connectedPort = nullptr;
}

void FlowInputPort::OnEndDrag()
{
	FlowPort::OnEndDrag();
	m_connectingInputPort = nullptr;
}
