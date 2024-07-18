#pragma once
#include "IBuffer.h"
#include "HeaderHelper.h"
#include "DefineUtility.h"

template<typename T>
class ConstantBuffer : public IBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

public:
	void InitializeForGPU(ID3D11Device* DeviceIn, const T& CPUDataIn);
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer()
{
	BUFFER_ALIGN_CHECK(sizeof(T));
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
}

template<typename T>
inline void ConstantBuffer<T>::InitializeForGPU(ID3D11Device* DeviceIn, const T& CPUDataIn)
{
	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = sizeof(T);
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = NULL;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	AutoZeroMemory(SubresourceData);
	SubresourceData.pSysMem = &CPUDataIn;
	SubresourceData.SysMemPitch = sizeof(T);
	SubresourceData.SysMemSlicePitch = SubresourceData.SysMemPitch;

	DeviceIn->CreateBuffer(&BufferDesc, &SubresourceData, Buffer.GetAddressOf());
}
