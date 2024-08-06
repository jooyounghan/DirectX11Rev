#pragma once
#include "IGuiVisitor.h"
#include "HeaderHelper.h"

#include <string>

class PSOObject;

class AObject
{
protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

public:
	virtual void Render(PSOObject* PSOObjectIn) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;

public:
	virtual void AcceptGui(IGuiVisitor* GuiVisitor) = 0;
};

