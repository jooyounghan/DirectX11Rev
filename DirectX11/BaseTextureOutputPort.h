#pragma once
#include "VariableOutputPort.h"
#include "BaseTextureAsset.h"

class BaseTextureOutputPort : public VariableOutputPort
{
public:
	BaseTextureOutputPort(
		const ImVec2& CenterPositionIn,
		const float& RadiusSizeIn,
		std::shared_ptr<BaseTextureAsset> BaseTextureAssetIn
	);
	virtual ~BaseTextureOutputPort();

protected:
	std::shared_ptr<BaseTextureAsset> BaseTextureAssetCached;
	MakeSetterGetter(BaseTextureAssetCached);

public:
	virtual bool IsConnectable(InputPort* PortIn) override;
};

