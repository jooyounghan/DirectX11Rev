#pragma once
#include "AUploadableBuffer.h"

class StructureBuffer : public AUploadableBuffer
{
public:
	StructureBuffer(const UINT& elementSize, const UINT& arrayCount);


protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_structuredSRV;

public:
	virtual void Initialize(ID3D11Device* device) override;
	virtual void Upload(
		ID3D11DeviceContext* deviceContext,
		const UINT& elementSize, const UINT& arrayCount,
		void* cpuDataIn
	) override;
};

