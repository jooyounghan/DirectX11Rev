#pragma once
#include "AUploadableBuffer.h"

class StructuredBuffer : public AUploadableBuffer
{
public:
	StructuredBuffer(const UINT& elementSize, const UINT& arrayCount);
	~StructuredBuffer() override = default;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_structuredSRV;

public:
	ID3D11ShaderResourceView* const GetSRV() const { return m_structuredSRV.Get(); }

public:
	virtual void Initialize(ID3D11Device* const device, D3D11_SUBRESOURCE_DATA* initialData) override;
	virtual void Upload(ID3D11DeviceContext* const deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn) override;
};

