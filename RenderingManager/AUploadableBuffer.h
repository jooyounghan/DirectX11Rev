#pragma once
#include "ABuffer.h"

#include <atomic>

class AUploadableBuffer : public ABuffer
{
public:
	AUploadableBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn);
	~AUploadableBuffer() override = default;

protected:
	std::atomic_bool m_isChanged = false;

public:
	inline bool IsChanged() { return m_isChanged.exchange(false); }
	inline void SetChanged(const bool& isChanged) { m_isChanged.store(isChanged); }

public:
	virtual void Upload(ID3D11DeviceContext* const deviceContext);
};

