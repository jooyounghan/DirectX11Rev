#pragma once
#include "IInformationDrawer.h"

class BoundingSphere;

class BoundingSphereInformationDrawer : AInformationDrawer<BoundingSphere>
{
public:
	BoundingSphereInformationDrawer(BoundingSphere* BoundingSphereIn)
		: AInformationDrawer(BoundingSphereIn) {};

public:
	virtual void DrawInformation() override;
};

