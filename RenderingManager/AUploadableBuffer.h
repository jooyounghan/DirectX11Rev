#pragma once
#include "ABuffer.h"

class AUploadableBuffer : public ABuffer
{
public:
	AUploadableBuffer(const UINT& elementSize, const UINT& arrayCount);
	~AUploadableBuffer() override = default;

public:
	virtual void Upload(ID3D11DeviceContext* const deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn) = 0;
};

