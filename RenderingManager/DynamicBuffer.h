#pragma once
#include "AUploadableBuffer.h"

class DynamicBuffer : public AUploadableBuffer
{
public:
	DynamicBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn);
	~DynamicBuffer() override = default;

public:
	virtual void InitializeBuffer(ID3D11Device* const device, const D3D11_SUBRESOURCE_DATA* initialData) override;
};

