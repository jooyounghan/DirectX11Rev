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
	void SetAnimationAssetInstance(const std::shared_ptr<AnimationAsset>& AnimationAssetInstanceIn);

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void Render() override final;

protected:
	virtual std::vector<ID3D11Buffer*> GetMeshObjectVSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectHSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectDSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectPSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectVSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectHSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectDSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectPSSRVs(const size_t& MaterialIdx) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

