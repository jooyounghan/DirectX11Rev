#pragma once
#include "IInformationDrawer.h"

class OrientedBoundingBox;

class BoundingOBBInformationDrawer : AInformationDrawer<OrientedBoundingBox>
{
public:
	BoundingOBBInformationDrawer(OrientedBoundingBox* OBBIn)
		: AInformationDrawer(OBBIn) {};

public:
	virtual void DrawInformation() override;
};

