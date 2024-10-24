#include "AssetVariableInputPort.h"

using namespace std;

AssetVariableInputPort::AssetVariableInputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn, 
	const string& AssetTypeIn
)
	: VariableInputPort(CenterPositionIn, RadiusSizeIn), PortAssetType(AssetTypeIn)
{
}

AssetVariableInputPort::~AssetVariableInputPort()
{
}
