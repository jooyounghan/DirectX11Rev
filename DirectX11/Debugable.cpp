#include "Debugable.h"


Debugable::Debugable(ID3D11Device* DeviceIn)
	: DebuggingColorBuffer(DeviceIn)
{
}

Debugable::~Debugable()
{
}

DXGI_FORMAT Debugable::GetIndexFormat()
{
	return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
}

void Debugable::Initialize(ID3D11Device* DeviceIn)
{
	VerticesBuffer.InitializeForGPU(DeviceIn, GetVertexCount(), Vertices.data());
	IndicesBuffer.InitializeForGPU(DeviceIn, GetIndexCount(), Indices.data());
}

void Debugable::UpdateColor(const XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn)
{
	DebuggingColorBuffer.Upload(DeviceContextIn, ColorIn);
}
