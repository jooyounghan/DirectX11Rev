#include "FloatVariableOutputPort.h"
#include "AssetVariableInputPort.h"

using namespace std;
using namespace DirectX;

string FloatVariableOutputPort::FloatVariableKind = "Float";

FloatVariableOutputPort::FloatVariableOutputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn
)
	: VariableOutputPort(CenterPositionIn, RadiusSizeIn)
{
}

FloatVariableOutputPort::~FloatVariableOutputPort()
{
}

bool FloatVariableOutputPort::IsConnectable(InputPort* PortIn)
{
	AssetVariableInputPort* AssetVariableInputNodeCached = dynamic_cast<AssetVariableInputPort*>(PortIn);
	if (AssetVariableInputNodeCached != nullptr)
	{
		if (AssetVariableInputNodeCached->GetPortAssetType() == FloatVariableOutputPort::FloatVariableKind)
		{
			return true;
		}
	}
	return false;
}
