#pragma once
#include "AAttachableObject.h"

class AMeshAsset;

class AMeshObject : public AAttachableObject
{
public:
	AMeshObject(MapAsset* MapAssetInstance);
	virtual ~AMeshObject();

public:
	virtual AMeshAsset* GetMeshAssetInstance() = 0;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render() = 0;
};

