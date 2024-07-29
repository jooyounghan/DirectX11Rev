#include "Debugable.h"

Debugable::Debugable()
{
}

Debugable::~Debugable()
{
}

DXGI_FORMAT Debugable::GetIndexFormat()
{
	return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
}

void Debugable::Initialize(ID3D11Device* DeviceIn)
{
	VerticesBuffer.InitializeForGPU(DeviceIn, GetVertexCount(), Vertices.data());
	IndicesBuffer.InitializeForGPU(DeviceIn, GetIndexCount(), Indices.data());
}

void Debugable::Render(PSOObject* PSOObjectIn)
{
}
