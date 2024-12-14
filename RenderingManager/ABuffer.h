#pragma once
#include "DirectXUtilities.h"

#include <wrl/client.h>
#include <d3d11.h>

class ABuffer
{
public:
	ABuffer(const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

protected:
	UINT m_elementSize;
	UINT m_arrayCount;
	void* m_cpuDataIn;

public:
	ID3D11Buffer* GetBuffer() { return m_buffer.Get(); }
	
public:
	virtual void Initialize(ID3D11Device* device) = 0;
};

