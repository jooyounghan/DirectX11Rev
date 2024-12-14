
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn, const D3D11_USAGE& usage)
	: ABuffer(elementSize, arrayCount, cpuDataIn), m_usage(usage)
{

}

void ConstantBuffer::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc;
	AutoZeroMemory(bufferDesc);

	bufferDesc.ByteWidth = m_elementSize * m_arrayCount;
	bufferDesc.Usage = m_usage;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = NULL;
	bufferDesc.MiscFlags = NULL;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	AutoZeroMemory(subresourceData);
	
	subresourceData.pSysMem = m_cpuDataIn;
	subresourceData.SysMemPitch = m_elementSize * m_arrayCount;
	subresourceData.SysMemSlicePitch = subresourceData.SysMemPitch;

	device->CreateBuffer(&bufferDesc, &subresourceData, m_buffer.GetAddressOf());
}
