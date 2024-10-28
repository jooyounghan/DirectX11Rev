#include "Float3VariableOutputPort.h"
#include "AssetVariableInputPort.h"

using namespace std;
using namespace DirectX;

string Float3VariableOutputPort::Float3VariableKind = "Float3";

Float3VariableOutputPort::Float3VariableOutputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn
)
	: VariableOutputPort(CenterPositionIn, RadiusSizeIn)
{
}

Float3VariableOutputPort::~Float3VariableOutputPort()
{
}

bool Float3VariableOutputPort::IsConnectable(InputPort* PortIn)
{
	AssetVariableInputPort* AssetVariableInputNodeCached = dynamic_cast<AssetVariableInputPort*>(PortIn);
	if (AssetVariableInputNodeCached != nullptr)
	{
		if (AssetVariableInputNodeCached->GetPortAssetType() == Float3VariableOutputPort::Float3VariableKind)
		{
			return true;
		}
	}
	return false;
}
