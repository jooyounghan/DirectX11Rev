#include "AttachableObject.h"

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

void AttachableObject::AcceptGui(IGuiVisitor* GuiVisitor)
{
	return GuiVisitor->Visit(this);
}
