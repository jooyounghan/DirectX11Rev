#pragma once
#include "AUploadableBuffer.h"

class StructuredBuffer : public AUploadableBuffer
{
public:
	StructuredBuffer(const UINT& elementSize, const UINT& arrayCount);


protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_structuredSRV;

public:
	virtual void Initialize(ID3D11Device* device) override;
	virtual void Upload(
		ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn
	) override;
};

