#pragma once
#include "IInformationDrawer.h"

class SkeletalMeshObject;
class AssetManager;

class SkeletalMeshObjectInformationDrawer : public AInformationDrawer<SkeletalMeshObject>
{
public:
	SkeletalMeshObjectInformationDrawer(SkeletalMeshObject* ObjectIn, AssetManager* AssetManagerIn);

protected:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void DrawInformation() override;
};

