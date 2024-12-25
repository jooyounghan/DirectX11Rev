#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(const UINT& elementSize, const UINT& arrayCount)
	: AUploadableBuffer(elementSize, arrayCount)
{
}

void StructuredBuffer::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	AutoZeroMemory(bufferDesc);

	bufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.CPUAccessFlags = NULL;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = m_elementSize;

	D3D11_BUFFER_DESC stagingBufferDesc;
	AutoZeroMemory(stagingBufferDesc);

	stagingBufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.BindFlags = NULL;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	stagingBufferDesc.MiscFlags = NULL;
	stagingBufferDesc.StructureByteStride = m_elementSize;

	device->CreateBuffer(&bufferDesc, NULL, m_buffer.GetAddressOf());
	device->CreateBuffer(&stagingBufferDesc, NULL, m_stagingBuffer.GetAddressOf());


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	AutoZeroMemory(srvDesc);
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.NumElements = m_arrayCount;
	device->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_structuredSRV.GetAddressOf());
}

void StructuredBuffer::Upload(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext, 
	const UINT& elementSize, 
	const UINT& arrayCount, 
	void* cpuDataIn
)
{
	if (elementSize != m_elementSize) return;
	if (arrayCount != m_arrayCount)
	{
		m_structuredSRV->Release();
		m_stagingBuffer->Release();
		m_buffer->Release();
		m_arrayCount = arrayCount;

		Initialize(device);
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	AutoZeroMemory(mappedResource);

	AssertIfFailed(deviceContext->Map(m_stagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &mappedResource));
	memcpy(mappedResource.pData, cpuDataIn, elementSize * arrayCount);
	deviceContext->Unmap(m_stagingBuffer.Get(), 0);
	deviceContext->CopyResource(m_buffer.Get(), m_stagingBuffer.Get());
}
