#include "AUploadableBuffer.h"

AUploadableBuffer::AUploadableBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn)
	: ABuffer(elementSize, arrayCount, cpuDataIn)
{
}

void AUploadableBuffer::Upload(ID3D11DeviceContext* const deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	AutoZeroMemory(mappedResource);

	AssertIfFailed(deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, m_cpuDataIn, m_elementSize * m_arrayCount);
	deviceContext->Unmap(m_buffer.Get(), 0);
}
