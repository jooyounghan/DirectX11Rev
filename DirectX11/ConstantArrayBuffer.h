#pragma once
#include "Buffer.h"
#include "HeaderHelper.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "DefineUtility.h"

template<typename T>
class ConstantArrayBuffer : public BaseBuffer
{
public:
	ConstantArrayBuffer();
	~ConstantArrayBuffer();

public:
	virtual void InitializeForGPU(
		const UINT& ArrayCountIn, 
		T* CPUDataPtr
	);
};

template<typename T>
inline ConstantArrayBuffer<T>::ConstantArrayBuffer()
{
}

template<typename T>
inline ConstantArrayBuffer<T>::~ConstantArrayBuffer()
{
}

template<typename T>
inline void ConstantArrayBuffer<T>::InitializeForGPU(const UINT& ArrayCountIn, T* CPUDataPtr)
{
	BUFFER_ALIGN_CHECK(ArrayCountIn * sizeof(T));

	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = ArrayCountIn * sizeof(T);
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = NULL;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	AutoZeroMemory(SubresourceData);
	SubresourceData.pSysMem = CPUDataPtr;
	SubresourceData.SysMemPitch = ArrayCountIn * sizeof(T);
	SubresourceData.SysMemSlicePitch = SubresourceData.SysMemPitch;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	Device->CreateBuffer(&BufferDesc, &SubresourceData, Buffer.GetAddressOf());
}
