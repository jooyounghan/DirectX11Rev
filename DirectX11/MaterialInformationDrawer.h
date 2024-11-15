#pragma once
#include "IInformationDrawer.h"

class AMeshObject;
class AssetManager;

class MaterialInformationDrawer : public AInformationDrawer<AMeshObject>
{
public:
	MaterialInformationDrawer(AMeshObject* ObjectIn);

public:
	virtual void DrawInformation() override;
};

