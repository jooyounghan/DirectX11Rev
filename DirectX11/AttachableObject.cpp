#include "AttachableObject.h"

AttachableObject::AttachableObject()
{
}

AttachableObject::~AttachableObject()
{
}

void AttachableObject::UpdateObject(const float& DeltaTimeIn, IObject* ParentObject)
{
	for (auto& ChildObject : ChildrenObject)
	{
		ChildObject->UpdateObject(DeltaTimeIn, ParentObject);
	}
}
