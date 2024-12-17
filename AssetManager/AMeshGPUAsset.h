#pragma once
#include "ConstantBuffer.h"

#include "IGPUAsset.h"
#include <vector>

class AMeshGPUAsset : public IGPUAsset
{
public:
	AMeshGPUAsset() = default;
	virtual ~AMeshGPUAsset();

protected:
	std::vector<ConstantBuffer*>	m_vertexBuffers;
	ConstantBuffer*					m_indexBuffer = nullptr;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() = 0;
	virtual ID3D11Buffer* GetIndexBuffer() = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;

protected:
	void ResetGPUAsset();
};

