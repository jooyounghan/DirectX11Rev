#pragma once
#include "APlaceable.h"

class RelativePlaceableObject : public AAttachable, public APlaceable
{
public:
	RelativePlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~RelativePlaceableObject();

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override {};
};

