#pragma once
#include "Buffer.h"
#include "AUploadableBuffer.h"

#include "HeaderHelper.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

template<typename T>
class StructuredBuffer : public AUploadableBuffer
{
public:
	StructuredBuffer(const size_t& DataCountIn);
	~StructuredBuffer();

protected:
	UINT TotalBufferSize;

protected:
	std::vector<T> StagedStructuredData;
	MakeGetter(StagedStructuredData);

public:
	void SetStagedData(const size_t& Index, const T& StagedDataIn);

public:
	virtual void Upload() override;
};

template<typename T>
inline StructuredBuffer<T>::StructuredBuffer(const size_t& DataCountIn)
	: TotalBufferSize(static_cast<UINT>(DataCountIn * sizeof(T)))
{
	StagedStructuredData.resize(DataCountIn);

	BUFFER_ALIGN_CHECK(TotalBufferSize);

	D3D11_BUFFER_DESC BufferDesc;
	AutoZeroMemory(BufferDesc);

	BufferDesc.ByteWidth = TotalBufferSize;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	BufferDesc.CPUAccessFlags = NULL;
	BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferDesc.StructureByteStride = sizeof(T);

	D3D11_BUFFER_DESC StagingBufferDesc;
	AutoZeroMemory(StagingBufferDesc);

	StagingBufferDesc.ByteWidth = BufferDesc.ByteWidth;
	StagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	StagingBufferDesc.BindFlags = NULL;
	StagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	StagingBufferDesc.MiscFlags = NULL;
	StagingBufferDesc.StructureByteStride = BufferDesc.StructureByteStride;

	ID3D11Device* Device = App::GGraphicPipeline->GetDevice();
	Device->CreateBuffer(&BufferDesc, NULL, Buffer.GetAddressOf());
	Device->CreateBuffer(&StagingBufferDesc, NULL, StagingBuffer.GetAddressOf());
}

template<typename T>
inline StructuredBuffer<T>::~StructuredBuffer()
{
}

template<typename T>
inline void StructuredBuffer<T>::SetStagedData(const size_t& Index, const T& StagedDataIn)
{
	if (StagedStructuredData.size() > Index)
	{
		StagedStructuredData[Index] = StagedDataIn;
		UpdateReadyEvent.Invoke(this);
	}
}

template<typename T>
inline void StructuredBuffer<T>::Upload()
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	AutoZeroMemory(MappedResource);
	AssertIfFailed(DeviceContextCached->Map(StagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &MappedResource));
	memcpy(MappedResource.pData, StagedStructuredData.data(), TotalBufferSize);
	DeviceContextCached->Unmap(StagingBuffer.Get(), 0);
	DeviceContextCached->CopyResource(Buffer.Get(), StagingBuffer.Get());
}
