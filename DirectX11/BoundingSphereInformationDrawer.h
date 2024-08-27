#pragma once
#include "IInformationDrawer.h"

class BoundingSphereObject;

class BoundingSphereInformationDrawer : AInformationDrawer<BoundingSphereObject>
{
public:
	BoundingSphereInformationDrawer(BoundingSphereObject* BoundingSphereIn)
		: AInformationDrawer(BoundingSphereIn) {};

public:
	virtual void DrawInformation() override;
};

