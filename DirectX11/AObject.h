#pragma once

#include "HeaderHelper.h"

#include <string>

class PSOObject;
class IGuiTopLevelVisitor;
class IGuiLowLevelVisitor;

class AObject
{
protected:
	std::string ObjectName;
	MakeGetter(ObjectName);

public:
	virtual void Render(PSOObject* PSOObjectIn) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;

public:
	virtual void AcceptGui(IGuiTopLevelVisitor* GuiVisitor) = 0;
	virtual void AcceptGui(IGuiLowLevelVisitor* GuiVisitor) = 0;
};
