#pragma once
#include "IInformationDrawer.h"

class OBBObject;

class BoundingOBBInformationDrawer : AInformationDrawer<OBBObject>
{
public:
	BoundingOBBInformationDrawer(OBBObject* OBBIn)
		: AInformationDrawer(OBBIn) {};

public:
	virtual void DrawInformation() override;
};

