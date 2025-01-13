#pragma once
#include "SerializeHelper.h"
#include "AMeshGPUAsset.h"
#include "IMeshPartsDataVisitor.h"

#include <DirectXMath.h>

class MeshPartsData : public ISerializable, public AMeshGPUAsset
{
public:
	MeshPartsData() = default;
	~MeshPartsData() override = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<DirectX::XMFLOAT3> m_normals;
	std::vector<uint32_t> m_indices;
	std::vector<uint32_t> m_vertexPartOffsets;
	std::vector<uint32_t> m_indexPartOffsets;

protected:
	const DirectX::XMFLOAT3 m_emptyFloat3 = DirectX::XMFLOAT3(0, 0, 0);
	const DirectX::XMFLOAT2 m_emptyFloat2 = DirectX::XMFLOAT2(0, 0);

public:
	inline const std::vector<DirectX::XMFLOAT3>& GetPositions() { return m_positions; }
	inline const std::vector<DirectX::XMFLOAT2>& GetUVTextures() { return m_uvTextures; }
	inline const std::vector<DirectX::XMFLOAT3>& GetNormals() { return m_normals; }
	inline const std::vector<uint32_t>& GetIndices() { return m_indices; }
	inline const std::vector<uint32_t>& GetVertexPartOffsets() { return m_vertexPartOffsets;}
	inline const std::vector<uint32_t>& GetIndexOffsets() { return m_indexPartOffsets; }
	inline size_t GetPartsCount() { return m_indexPartOffsets.size(); }

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
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IMeshPartsDataVisitor& visitor) = 0;
};