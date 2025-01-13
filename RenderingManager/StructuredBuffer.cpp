#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(const UINT& elementSize, const UINT& arrayCount)
	: AUploadableBuffer(elementSize, arrayCount)
{
}

void StructuredBuffer::Initialize(ID3D11Device* const device, D3D11_SUBRESOURCE_DATA* initialData)
{
	D3D11_BUFFER_DESC bufferDesc;
	AutoZeroMemory(bufferDesc);

	bufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_elementSize;

	device->CreateBuffer(&bufferDesc, initialData, m_buffer.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	AutoZeroMemory(srvDesc);
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.NumElements = m_arrayCount;
	device->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_structuredSRV.GetAddressOf());
}


void StructuredBuffer::Upload(ID3D11DeviceContext* const deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn)
{
	if (elementSize != m_elementSize) return;
	if (arrayCount != m_arrayCount) return;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	AutoZeroMemory(mappedResource);

	AssertIfFailed(deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, cpuDataIn, elementSize * arrayCount);
	deviceContext->Unmap(m_buffer.Get(), 0);
}
