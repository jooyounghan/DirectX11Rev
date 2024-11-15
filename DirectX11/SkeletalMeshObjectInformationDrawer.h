#pragma once
#include "IInformationDrawer.h"

class SkeletalMeshObject;
class AssetManager;

class SkeletalMeshObjectInformationDrawer : public AInformationDrawer<SkeletalMeshObject>
{
public:
	SkeletalMeshObjectInformationDrawer(SkeletalMeshObject* ObjectIn);

public:
	virtual void DrawInformation() override;
};

