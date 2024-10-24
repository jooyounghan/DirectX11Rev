#pragma once
#include "VariableInputPort.h"
#include "AAssetFile.h"
#include "HeaderHelper.h"

class AssetVariableInputPort : public VariableInputPort
{
public:
	AssetVariableInputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn,
		const std::string& AssetTypeIn
	);
	virtual ~AssetVariableInputPort();

protected:
	std::string PortAssetType;
	MakeGetter(PortAssetType);
};

