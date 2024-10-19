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
		const EAssetType& AssetTypeIn
	);
	virtual ~AssetVariableInputPort();

protected:
	EAssetType PortAssetType;
	MakeGetter(PortAssetType);
};

