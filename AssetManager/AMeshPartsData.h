#pragma once
#include "SerializeHelper.h"
#include "AMeshData.h"
#include "IMeshPartsDataVisitor.h"


class AMeshPartsData : public ISerializable, public AMeshData
{
public:
	AMeshPartsData() = default;
	~AMeshPartsData() override = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_normals;
	ConstantBuffer* m_normalBuffer = nullptr;

protected:
	std::vector<uint32_t> m_vertexPartOffsets;
	std::vector<uint32_t> m_indexPartOffsets;

public:
	inline ConstantBuffer* GetNormalBuffer() const { return m_normalBuffer; }

public:
	void SetNormalBuffer(ConstantBuffer* normalBuffer);

public:
	inline const std::vector<DirectX::XMFLOAT3>& GetNormals() const { return m_normals; }
	inline const std::vector<uint32_t>& GetIndices() const { return m_indices; }
	inline const std::vector<uint32_t>& GetVertexPartOffsets() const { return m_vertexPartOffsets;}
	inline const std::vector<uint32_t>& GetIndexOffsets() const { return m_indexPartOffsets; }
	inline size_t GetPartsCount() const { return m_indexPartOffsets.size(); }

public:
	const DirectX::XMFLOAT3& GetNormal(const size_t& index) const;

public:
	void AddNormal(const float& x, const float& y, const float& z);
	void AddIndex(const uint32_t& offset, const uint32_t index);
	void AddPartOffsets(const uint32_t& vertexOffset, const uint32_t& indexOffset);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IMeshPartsDataVisitor& visitor) = 0;

protected:
	virtual void ResetMeshData() override;
};