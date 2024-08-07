#include "AttachableObject.h"
#include "IGuiTopLevelVisitor.h"

AttachableObject::AttachableObject()
{
}

AttachableObject::~AttachableObject()
{
}

void AttachableObject::UpdateObject(const float& DeltaTimeIn)
{
	// Update Something From the Parent
}

void AttachableObject::AcceptGui(IGuiTopLevelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}
