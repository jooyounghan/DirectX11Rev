#pragma once
#include "IObject.h"

class AttachableObject : public IObject
{
public:
	AttachableObject();
	virtual ~AttachableObject();

public:
	virtual void UpdateObject(const float& DeltaTimeIn, IObject* ParentObject = nullptr) override;
};

