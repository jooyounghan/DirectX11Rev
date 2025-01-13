#pragma once
#include "DirectXUtilities.h"

#include <wrl/client.h>
#include <d3d11.h>

class ABuffer
{
public:
	ABuffer(const UINT& elementSize, const UINT& arrayCount, const void* cpuDataIn);
	virtual ~ABuffer() { m_buffer.Reset(); };

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

protected:
	UINT m_elementSize;
	UINT m_arrayCount;
	const void* m_cpuDataIn;

public:
	D3D11_SUBRESOURCE_DATA GetSubResourceData() const;
	inline ID3D11Buffer* GetBuffer() const { return m_buffer.Get(); }
	
public:
	virtual void InitializeBuffer(ID3D11Device* const device, const D3D11_SUBRESOURCE_DATA* initialData) = 0;
};

