#pragma once
#include "ABuffer.h"

class ConstantBuffer : public ABuffer
{
public:
	ConstantBuffer(
		const UINT& elementSize, const UINT& arrayCount, 
		const void* cpuDataIn,
		const D3D11_BIND_FLAG& bindFlag = D3D11_BIND_CONSTANT_BUFFER,
		const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE
	);
	~ConstantBuffer() override = default;

protected:
	D3D11_USAGE m_usage;
	D3D11_BIND_FLAG m_bindFlag;

public:
	virtual void InitializeBuffer(ID3D11Device* device, const D3D11_SUBRESOURCE_DATA* initialData) override;
};

