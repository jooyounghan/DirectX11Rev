#pragma once
#include "stdint.h"
#include "ConstantArrayBuffer.h"

template<typename T>
class VertexBuffer : public ConstantArrayBuffer<T>
{
public:
	VertexBuffer();
	~VertexBuffer();

public:
	virtual void InitializeForGPU(
		const UINT& ArrayCountIn,
		T* CPUDataPtr
	) override;
};

template<typename T>
inline VertexBuffer<T>::VertexBuffer()
{
}

template<typename T>
inline VertexBuffer<T>::~VertexBuffer()
{
}

template<typename T>
inline void VertexBuffer<T>::InitializeForGPU(
	const UINT& ArrayCountIn,
	T* CPUDataPtr
)
{
	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = ArrayCountIn * sizeof(T);
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = NULL;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	AutoZeroMemory(SubresourceData);
	SubresourceData.pSysMem = CPUDataPtr;
	SubresourceData.SysMemPitch = ArrayCountIn * sizeof(T);
	SubresourceData.SysMemSlicePitch = SubresourceData.SysMemPitch;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	Device->CreateBuffer(&BufferDesc, &SubresourceData, BaseBuffer::Buffer.GetAddressOf());
}
