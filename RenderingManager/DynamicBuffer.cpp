
#include "DynamicBuffer.h"

DynamicBuffer::DynamicBuffer(const UINT& elementSize, const UINT& arrayCount)
	: AUploadableBuffer(elementSize, arrayCount)
{

}

void DynamicBuffer::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	AutoZeroMemory(bufferDesc);

	bufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = NULL;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC stagingBufferDesc;
	AutoZeroMemory(stagingBufferDesc);

	stagingBufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.BindFlags = NULL;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	stagingBufferDesc.MiscFlags = NULL;
	stagingBufferDesc.StructureByteStride = 0;

	device->CreateBuffer(&bufferDesc, NULL, m_buffer.GetAddressOf());
	device->CreateBuffer(&stagingBufferDesc, NULL, m_stagingBuffer.GetAddressOf());
}

void DynamicBuffer::Upload(ID3D11DeviceContext* deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn)
{
	if (elementSize != m_elementSize) return;
	if (arrayCount != m_arrayCount) return;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	AutoZeroMemory(mappedResource);

	AssertIfFailed(deviceContext->Map(m_stagingBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, cpuDataIn, m_elementSize * m_arrayCount);
	deviceContext->Unmap(m_stagingBuffer.Get(), 0);
	deviceContext->CopyResource(m_buffer.Get(), m_stagingBuffer.Get());
}
