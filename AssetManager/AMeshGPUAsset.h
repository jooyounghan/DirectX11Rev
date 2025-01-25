#pragma once
#include <vector>
#include <d3d11.h>

class ConstantBuffer;

class AMeshGPUAsset
{
public:
	AMeshGPUAsset() = default;
	virtual ~AMeshGPUAsset();

protected:
	std::vector<ConstantBuffer*>	m_vertexBuffers;
	ConstantBuffer*					m_indexBuffer = nullptr;

public:
	inline const std::vector<ConstantBuffer*>& GetVertexBuffers() const { return m_vertexBuffers; }
	inline ConstantBuffer* GetIndexBuffer() const { return m_indexBuffer; }
	std::vector<ID3D11Buffer*> GetD3D11VertexBuffers() const;
	ID3D11Buffer* GetD3D11IndexBuffer() const;

public:
	inline void AddVertexBuffer(ConstantBuffer* constantBuffer) { m_vertexBuffers.emplace_back(constantBuffer); }
	void SetIndexBuffer(ConstantBuffer* indexBuffer);

public:
	virtual std::vector<UINT> GetStrides() const = 0;
	virtual std::vector<UINT> GetOffsets() const = 0;

protected:
	void ResetGPUAsset();
};

