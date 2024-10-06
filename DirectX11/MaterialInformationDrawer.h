#pragma once
#include "IInformationDrawer.h"

class AMeshObject;
class AssetManager;

class MaterialInformationDrawer : public AInformationDrawer<AMeshObject>
{
public:
	MaterialInformationDrawer(AMeshObject* ObjectIn, AssetManager* AssetManagerIn);

protected:
	AssetManager* AssetManagerCached = nullptr;

public:
	virtual void DrawInformation() override;
};

