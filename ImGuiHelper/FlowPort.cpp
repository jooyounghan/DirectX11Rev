#include "FlowPort.h"
#include "NodeConstant.h"

FlowPort* FlowPort::m_connectingInputPort = nullptr;
FlowPort* FlowPort::m_connectingOutputPort = nullptr;