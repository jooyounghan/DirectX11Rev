
#include "ABuffer.h"

ABuffer::ABuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn)
	: m_elementSize(elementSize), m_arrayCount(arrayCount), m_cpuDataIn(cpuDataIn)
{
}

D3D11_SUBRESOURCE_DATA ABuffer::GetSubResourceData() const
{
	D3D11_SUBRESOURCE_DATA subresourceData;
	AutoZeroMemory(subresourceData);

	subresourceData.pSysMem = m_cpuDataIn;
	return subresourceData;
}