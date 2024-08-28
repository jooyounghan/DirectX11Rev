#pragma once
#include "IInformationDrawer.h"

class MeshObject;
class AssetManager;

class MeshObjectInformationDrawer : public AInformationDrawer<MeshObject>
{
public:
	MeshObjectInformationDrawer(MeshObject* ObjectIn, AssetManager* AssetManagerIn);

protected:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void DrawInformation() override;
};

