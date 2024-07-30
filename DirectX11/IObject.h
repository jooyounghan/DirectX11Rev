#pragma once
#include "DefineType.h"
#include "HeaderHelper.h"
#include <vector>
#include <memory>

class IObject
{
public:
	IObject();

public:
	SPosition4D	Position;
	SAngle		Angle;
	float		Scale;

protected:
	bool IsRelativeToParent = false;

public:
	std::vector<IObject*> ChildrenObject;

public:
	DirectX::XMVECTOR GetRotationQuat() const;
	
public:
	DirectX::XMMATRIX GetScaleMatrix() { return DirectX::XMMatrixScaling(Scale, Scale, Scale); }
	DirectX::XMMATRIX GetRotartionMatrix() { return DirectX::XMMatrixRotationRollPitchYaw(Angle.Pitch, Angle.Yaw, Angle.Roll); }
	DirectX::XMMATRIX GetTranslationMatrix() { return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z); }

public:
	DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) = 0;
};

