#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class ConstantBuffer;

class AMeshData
{
public:
	AMeshData() = default;
	virtual ~AMeshData();

protected:
	ConstantBuffer*					m_vertexBuffer = nullptr;
	ConstantBuffer*					m_uvBuffer = nullptr;
	ConstantBuffer*					m_indexBuffer = nullptr;

protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<uint32_t> m_indices;

protected:
	static DirectX::XMFLOAT3 EmptyFloat3;
	static DirectX::XMFLOAT2 EmptyFloat2;

public:
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffers() const = 0;
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffersForDepthTest() const = 0;
	std::vector<ID3D11Buffer*> GetVertexBuffers() const;
	std::vector<ID3D11Buffer*> GetVertexBuffersForDepthTest() const;

public:
	inline ConstantBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
	inline ConstantBuffer* GetUVBuffer() const { return m_uvBuffer; }
	inline ConstantBuffer* GetIndexBuffer() const { return m_indexBuffer; }

public:
	void SetVertexBuffer(ConstantBuffer* vertexBuffer);
	void SetUVBuffer(ConstantBuffer* uvBuffer);
	void SetIndexBuffer(ConstantBuffer* indexBuffer);

public:
	inline const std::vector<DirectX::XMFLOAT3>& GetPositions() const { return m_positions; }
	inline const std::vector<DirectX::XMFLOAT2>& GetUVTextures()const { return m_uvTextures; }

public:
	const DirectX::XMFLOAT3& GetPosition(const size_t& index) const;
	const DirectX::XMFLOAT2& GetUVTextureCoord(const size_t& index) const;

public:
	void AddPosition(const float& x, const float& y, const float& z);
	void AddUVTexture(const float& u, const float& v);

public:
	virtual std::vector<UINT> GetStrides() const = 0;
	virtual std::vector<UINT> GetOffsets() const = 0;
	virtual std::vector<UINT> GetStridesForDepthTest() const = 0;
	virtual std::vector<UINT> GetOffsetsForDepthTest() const = 0;

protected:
	virtual void ResetMeshData();

private:
	std::vector<ID3D11Buffer*> GetBuffersFromVertexConstantBuffers(const std::vector<ConstantBuffer*>& vertexConstantBuffers) const;
};

