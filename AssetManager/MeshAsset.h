#pragma once
#include "Asset.h"
#include "ModelMaterialAsset.h"
#include "AMeshGPUAsset.h"

#include <map>

class MeshPartsData : public ISerializable, public AMeshGPUAsset
{
public:
	MeshPartsData() = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<DirectX::XMFLOAT3> m_normals;
	const DirectX::XMFLOAT3 m_emptyFloat3 = DirectX::XMFLOAT3(0, 0, 0);
	const DirectX::XMFLOAT2 m_emptyFloat2 = DirectX::XMFLOAT2(0, 0);

protected:
	std::vector<uint32_t> m_indices;
	
protected:
	std::vector<uint32_t> m_partVertexOffsets;
	std::vector<uint32_t> m_partIndexOffsets;

public:
	size_t GetPartsCount() { return m_partIndexOffsets.size(); }

public:
	const DirectX::XMFLOAT3& GetPosition(const size_t& index);
	const DirectX::XMFLOAT2& GetUVTextureCoord(const size_t& index);
	const DirectX::XMFLOAT3& GetNormal(const size_t& index);

public:
	void AddPosition(const float& x, const float& y, const float& z);
	void AddUVTexture(const float& u, const float& v);
	void AddNormal(const float& x, const float& y, const float& z);
	void AddIndex(const uint32_t& offset, const uint32_t index);
	void AddPartOffsets(const uint32_t& vertexOffset, const uint32_t& indexOffset);

public:
	const std::vector<DirectX::XMFLOAT3>& GetPositions();
	const std::vector<uint32_t>& GetIndices();

public:
	const std::vector<uint32_t>& GetVertexOffsets();
	const std::vector<uint32_t>& GetIndexOffsets();

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class AMeshAsset : public AAsset, public IGPUAsset
{
public:
	AMeshAsset() = default;
	AMeshAsset(const std::string& assetName);
	virtual ~AMeshAsset();

protected:
	std::vector<std::string> m_defaultMaterialNames;
	std::vector<ModelMaterialAsset*> m_defaultMaterials;

public:
	inline const std::vector<ModelMaterialAsset*>& GetDefaultMaterials() const { return m_defaultMaterials; }

public:
	void UpdateDefaultMaterialAsset(IModelMaterialProvider& provider);

public:
	virtual size_t GetLODCount() = 0;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) = 0;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};
