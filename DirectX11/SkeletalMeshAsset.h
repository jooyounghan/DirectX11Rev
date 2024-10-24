#pragma once
#include "ANBTMeshAsset.h"

class BoneAsset;

constexpr const char* SkeletalMeshAssetOutPath = ".\\Assets\\SkeletalMesh\\";

class SkeletalMeshAsset : public ANBTMeshAsset
{
public:
	SkeletalMeshAsset(const std::string& AssetNameIn, bool LoadFromAsset);
	virtual ~SkeletalMeshAsset();

public:
	static std::string SkeletalMeshAssetKind;

protected:
	std::shared_ptr<BoneAsset> LinkedBoneAsset = nullptr;
	MakeSetterGetter(LinkedBoneAsset)

public:
	std::vector<Vertexable<DirectX::XMFLOAT4>> BlendWeightPerLOD;
	std::vector<Vertexable<DirectX::XMINT4>> BlendIndexPerLOD;

public:
	virtual void SetLODCount(const size_t& LODCountIn) override;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers(const size_t& LODLevelIn) override;
	virtual std::vector<UINT> GetStrides() override;
	virtual std::vector<UINT> GetOffsets() override;

public:
	virtual void Initialize() override;

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;

protected:
	virtual void SerializeBaseMeshData(FILE* FileIn) override;
	virtual void DeserializeBaseMeshData(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

