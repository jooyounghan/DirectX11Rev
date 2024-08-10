#pragma once
#include "IInformationDrawer.h"

class MeshObject;


class MeshObjectInformationDrawer : public AInformationDrawer<MeshObject>
{
public:
	MeshObjectInformationDrawer(MeshObject* ObjectIn)
		: AInformationDrawer(ObjectIn) {};

public:
	virtual void DrawInformation() override;
};

