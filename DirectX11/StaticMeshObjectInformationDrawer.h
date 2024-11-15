#pragma once
#include "IInformationDrawer.h"

class StaticMeshObject;
class AssetManager;

class StaticMeshObjectInformationDrawer : public AInformationDrawer<StaticMeshObject>
{
public:
	StaticMeshObjectInformationDrawer(StaticMeshObject* ObjectIn);

public:
	virtual void DrawInformation() override;
};

