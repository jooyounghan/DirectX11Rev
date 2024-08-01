#pragma once
#include "APlaceable.h"

class PlaceableObject : public APlaceable
{
public:
	PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~PlaceableObject();

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override {};
};

