#pragma once
#include "ABuffer.h"

class AUploadableBuffer : public ABuffer
{
public:
	AUploadableBuffer(const UINT& elementSize, const UINT& arrayCount);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_stagingBuffer;

public:
	virtual void Upload(
		ID3D11DeviceContext* deviceContext,
		const UINT& elementSize, const UINT& arrayCount,
		void* cpuDataIn
	) = 0;
};

