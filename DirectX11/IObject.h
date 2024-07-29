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

protected:
	std::vector<std::unique_ptr<IObject>> ChildrenObject;

public:
	DirectX::XMVECTOR GetRotationQuat() const;
	DirectX::XMMATRIX GetTransformation() const;

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) = 0;
};

