#include "Debugable.h"

Debugable::Debugable(ID3D11Device* DeviceIn)
{
}

Debugable::~Debugable()
{
}

DXGI_FORMAT Debugable::GetIndexFormat()
{
	return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
}

void Debugable::Initialize()
{
	VerticesBuffer.InitializeForGPU(GetVertexCount(), Vertices.data());
	IndicesBuffer.InitializeForGPU(GetIndexCount(), Indices.data());
}