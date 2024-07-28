#pragma once
#include "DefineType.h"
#include "HeaderHelper.h"

class IObject
{
public:
	IObject();

public:
	SPosition4D	Position;
	SAngle		Angle;
	float		Scale;

protected:
	DirectX::XMVECTOR GetRotationQuat() const;
	DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParnetObject = nullptr) = 0;
};

