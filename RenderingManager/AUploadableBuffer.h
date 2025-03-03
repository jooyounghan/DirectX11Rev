#pragma once
#include "ABuffer.h"
#include "AtomicFlag.h"

class AUploadableBuffer : public ABuffer
{
public:
	AUploadableBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn);
	~AUploadableBuffer() override = default;

protected:
	AtomicFlag m_isChanged = false;

public:
	inline AtomicFlag& GetBufferChangedFlag() { return m_isChanged; }

public:
	virtual void Upload(ID3D11DeviceContext* const deviceContext);
};

