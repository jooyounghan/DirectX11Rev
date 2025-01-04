#include "FlowInputPort.h"
#include "FlowOutputPort.h"

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
