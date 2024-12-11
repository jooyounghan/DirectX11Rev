#pragma once
#include "AUploadableBuffer.h"

class DynamicBuffer : public AUploadableBuffer
{
public:
	DynamicBuffer(const UINT& elementSize, const UINT& arrayCount);

public:
	virtual void Initialize(ID3D11Device* device) override;
	virtual void Upload(
		ID3D11DeviceContext* deviceContext, 
		const UINT& elementSize, const UINT& arrayCount, 
		void* cpuDataIn
	) override;
};

