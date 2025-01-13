#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn)
	: AUploadableBuffer(elementSize, arrayCount, cpuDataIn)
{
}

void StructuredBuffer::InitializeBuffer(ID3D11Device* const device, const D3D11_SUBRESOURCE_DATA* initialData)
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