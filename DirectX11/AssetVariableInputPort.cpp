#include "AssetVariableInputPort.h"

AssetVariableInputPort::AssetVariableInputPort(
	const ImVec2& CenterPositionIn, 
	const float& RadiusSizeIn, 
	const EAssetType& AssetTypeIn
)
	: VariableInputPort(CenterPositionIn, RadiusSizeIn), PortAssetType(AssetTypeIn)
{
}

AssetVariableInputPort::~AssetVariableInputPort()
{
}
