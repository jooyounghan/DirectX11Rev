#pragma once
#include "AAttachableObject.h"
#include "MaterialAsset.h"

class AMeshAsset;
class MaterialAsset;
class PSOObject;

class AMeshObject : public AAttachableObject
{
public:
	AMeshObject(
		const std::string& AttachableKindIn
	);
	virtual ~AMeshObject();

protected:
	std::vector<std::shared_ptr<MaterialAsset>> MaterialAssetInstances;
	MakeGetter(MaterialAssetInstances);

public:
	void SetMaterialAsset(const size_t& MaterialIdx, std::shared_ptr<MaterialAsset> MaterialAssetIn);
	MaterialAsset* GetMaterialAssetInstance(const size_t& MaterialIdx);

public:
	virtual AMeshAsset* GetMeshAssetInstance() = 0;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	size_t GetLODLevel(
		const DirectX::XMFLOAT3& From, 
		const float& MaxDistance, 
		const size_t& MaxLODCount, 
		const size_t SteepLevel
	);

protected:
	PSOObject* MeshObjectPSOCached = nullptr;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void Render(MapAsset* MapAssetIn) override;

protected:
	virtual std::vector<ID3D11Buffer*> GetMeshObjectVSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectHSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectDSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectPSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectVSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectHSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectDSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectPSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) = 0;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn) override;

};

