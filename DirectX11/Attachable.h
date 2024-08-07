#pragma once
#include "AObject.h"

class APlaceable;

class AAttachable : virtual public AObject
{
public:
	AAttachable() = default;
	virtual ~AAttachable() = default;

protected:
	APlaceable* ParentObject = nullptr;
	MakeSetterGetter(ParentObject);

public:
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;
	virtual void Render(PSOObject* PSOObjectIn) = 0;

public:
	virtual void AcceptGui(IGuiTopLevelVisitor* GuiVisitor) = 0;
};