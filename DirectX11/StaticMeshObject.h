#pragma once
#include "AMeshObject.h"

class StaticMeshAsset;

class StaticMeshObject : public AMeshObject
{
public:
	StaticMeshObject(std::shared_ptr<StaticMeshAsset> SkeletalMeshAssetInstanceIn);
	virtual ~StaticMeshObject();

protected:
	std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstance = nullptr;
	MakeSmartPtrSetterGetter(StaticMeshAssetInstance);

public:
	static const char* StaticMeshObjectIdentifier;

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

