#pragma once
#include "AActor.h"
#include "HeaderHelper.h"

class SkeletalMeshAsset;
class SkeletalMeshObject;

class SkeletalMeshObjectActor : public AActor
{
public:
	SkeletalMeshObjectActor(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> MeshAssetInstanceIn);
	~SkeletalMeshObjectActor();

protected:
	std::unique_ptr<SkeletalMeshObject> SkeletalMeshObjectInstance;
	MakeSmartPtrGetter(SkeletalMeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render() override final;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};