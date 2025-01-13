
#include "DynamicBuffer.h"

DynamicBuffer::DynamicBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn)
	: AUploadableBuffer(elementSize, arrayCount, cpuDataIn)
{

}

void DynamicBuffer::InitializeBuffer(ID3D11Device* const device, const D3D11_SUBRESOURCE_DATA* initialData)
{
	D3D11_BUFFER_DESC bufferDesc;
	AutoZeroMemory(bufferDesc);

	bufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&bufferDesc, initialData, m_buffer.GetAddressOf());

}