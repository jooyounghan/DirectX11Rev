#pragma once
#include "PlaceableObject.h"


class AttachableObject : public PlaceableObject
{
public:
	AttachableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn);
	virtual ~AttachableObject();

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) override;
};