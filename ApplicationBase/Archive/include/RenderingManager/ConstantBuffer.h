#pragma once
#include "ABuffer.h"

class ConstantBuffer : public ABuffer
{
public:
	ConstantBuffer(const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);

protected:
	D3D11_USAGE m_usage;

public:
	virtual void Initialize(ID3D11Device* device) override;
};

