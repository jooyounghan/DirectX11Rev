#pragma once
#include "AMeshObject.h"

class SkeletalMeshAsset;
class PSOObject;

class SkeletalMeshObject : public AMeshObject
{
public:
	SkeletalMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn);
	virtual ~SkeletalMeshObject();

protected:
	std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstance = nullptr;
	MakeSmartPtrSetterGetter(SkeletalMeshAssetInstance);

public:
	static const char* SkeletalMeshObjectIdentifier;
	
public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render() override final;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

