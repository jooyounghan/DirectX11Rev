#pragma once
#include "IObject.h"
#include "HeaderHelper.h"

class APlaceable;

class AAttachable : virtual public IObject
{
public:
	AAttachable() = default;
	virtual ~AAttachable() = default;

protected:
	APlaceable* ParentObject;
	MakeSetterGetter(ParentObject);

public:
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;
	virtual void Render(PSOObject* PSOObjectIn) = 0;
};