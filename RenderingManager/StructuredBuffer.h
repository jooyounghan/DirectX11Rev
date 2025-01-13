#pragma once
#include "AUploadableBuffer.h"

class StructuredBuffer : public AUploadableBuffer
{
public:
	StructuredBuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn);
	~StructuredBuffer() override = default;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_structuredSRV;

public:
	ID3D11ShaderResourceView* const GetSRV() const { return m_structuredSRV.Get(); }

public:
	virtual void InitializeBuffer(ID3D11Device* const device, const D3D11_SUBRESOURCE_DATA* initialData) override;
};

