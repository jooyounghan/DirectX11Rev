#pragma once
#include "AMeshObject.h"
#include "AnimationPlayer.h"

class SkeletalMeshAsset;
class PSOObject;
class AnimationPlayer;

class SkeletalMeshObject : public AMeshObject
{
public:
	SkeletalMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn);
	virtual ~SkeletalMeshObject();

public:
	static std::string SkeletalMeshObjectKind;

protected:
	std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstance = nullptr;
	MakeSmartPtrGetter(SkeletalMeshAssetInstance);

protected:
	AnimationPlayer AnimationPlayerInstance;
	MakeGetter(AnimationPlayerInstance);

public:
	void SetSkeletalMeshAssetInstance(const std::shared_ptr<SkeletalMeshAsset>& SkeletalMeshAssetInstanceIn);

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render() override final;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

