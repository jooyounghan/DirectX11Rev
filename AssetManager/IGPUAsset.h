#pragma once
#include <d3d11.h>

class IGPUAsset
{
public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) = 0;
};