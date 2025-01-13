
#include "DynamicBuffer.h"

DynamicBuffer::DynamicBuffer(const UINT& elementSize, const UINT& arrayCount)
	: AUploadableBuffer(elementSize, arrayCount)
{

}

void DynamicBuffer::Initialize(ID3D11Device* const device, D3D11_SUBRESOURCE_DATA* initialData)
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

void DynamicBuffer::Upload(ID3D11DeviceContext* const deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn)
{
	if (elementSize != m_elementSize) return;
	if (arrayCount != m_arrayCount) return;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	AutoZeroMemory(mappedResource);

	AssertIfFailed(deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, cpuDataIn, m_elementSize * m_arrayCount);
	deviceContext->Unmap(m_buffer.Get(), 0);
}