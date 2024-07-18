#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::InitializeForGPU(
	ID3D11Device* DeviceIn, 
	const UINT& ArrayCountIn,
	uint32_t* CPUDataPtr
)
{
	IndicesCount = static_cast<UINT>(ArrayCountIn);

	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = ArrayCountIn * sizeof(uint32_t);
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = NULL;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	AutoZeroMemory(SubresourceData);
	SubresourceData.pSysMem = CPUDataPtr;
	SubresourceData.SysMemPitch = ArrayCountIn * sizeof(uint32_t);
	SubresourceData.SysMemSlicePitch = SubresourceData.SysMemPitch;

	DeviceIn->CreateBuffer(&BufferDesc, &SubresourceData, Buffer.GetAddressOf());
}
