#pragma once
#include "stdint.h"
#include "ConstantArrayBuffer.h"

class IndexBuffer : public ConstantArrayBuffer<uint32_t>
{
public:
	IndexBuffer();
	~IndexBuffer();

public:
	virtual void InitializeForGPU(
		ID3D11Device* DeviceIn, 
		const UINT& ArrayCountIn,
		uint32_t* CPUDataPtr
	) override;

protected:
	UINT IndicesCount = 0;
	MakeGetter(IndicesCount);
};