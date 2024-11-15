#pragma once
#include "AActor.h"
#include "HeaderHelper.h"

class SkeletalMeshAsset;
class SkeletalMeshObject;

class SkeletalMeshObjectActor : public AActor
{
public:
	SkeletalMeshObjectActor(std::shared_ptr<SkeletalMeshAsset> MeshAssetInstanceIn);
	~SkeletalMeshObjectActor();

public:
	static std::string SkeletalMeshObjectActorKind;

protected:
	std::unique_ptr<SkeletalMeshObject> SkeletalMeshObjectInstance;
	MakeSmartPtrGetter(SkeletalMeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};