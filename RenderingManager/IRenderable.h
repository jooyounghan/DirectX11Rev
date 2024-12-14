#pragma once
#include <d3d11.h>
#include <vector>

class IRenderable
{
public:
	virtual ID3D11Buffer* GetIndexBuffer() = 0;
	virtual DXGI_FORMAT GetIndexFormat() = 0;

public:
	virtual std::vector<ID3D11Buffer*> GetVertexBuffers() = 0;
	virtual std::vector<UINT> GetStrides() = 0;
	virtual std::vector<UINT> GetOffsets() = 0;
};

