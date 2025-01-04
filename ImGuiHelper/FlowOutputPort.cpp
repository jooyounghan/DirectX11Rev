#include "FlowOutputPort.h"
#include "FlowNode.h"

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
