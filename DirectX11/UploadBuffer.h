#pragma once
#include "Buffer.h"
#include "IUploadableBuffer.h"

#include "HeaderHelper.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

template<typename T>
class UploadBuffer : public Buffer, public IUploadableBuffer<T>
{
public:
	UploadBuffer();
	~UploadBuffer();

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> StagingBuffer;

protected:
	ID3D11DeviceContext* DeviceContextCached;

public:
	virtual void Upload(const T& CpuDataIn) override;
};

template<typename T>
inline UploadBuffer<T>::UploadBuffer()
	: DeviceContextCached(App::GGraphicPipeline->GetDeviceContext())
{
	BUFFER_ALIGN_CHECK(sizeof(T));

	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = sizeof(T);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = NULL;
	BufferDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC StagingBufferDesc;
	AutoZeroMemory(StagingBufferDesc);

	StagingBufferDesc.ByteWidth = sizeof(T);
	StagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	StagingBufferDesc.BindFlags = NULL;
	StagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	StagingBufferDesc.MiscFlags = NULL;
	StagingBufferDesc.StructureByteStride = 0;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	Device->CreateBuffer(&BufferDesc, NULL, Buffer.GetAddressOf());
	Device->CreateBuffer(&StagingBufferDesc, NULL, StagingBuffer.GetAddressOf());
}

template<typename T>
inline UploadBuffer<T>::~UploadBuffer()
{
}

template<typename T>
inline void UploadBuffer<T>::Upload(const T& CpuDataIn)
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	AutoZeroMemory(MappedResource);
	AssertIfFailed(DeviceContextCached->Map(StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &MappedResource));
	memcpy(MappedResource.pData, &CpuDataIn, sizeof(T));
	DeviceContextCached->Unmap(StagingBuffer.Get(), 0);
	DeviceContextCached->CopyResource(Buffer.Get(), StagingBuffer.Get());
}