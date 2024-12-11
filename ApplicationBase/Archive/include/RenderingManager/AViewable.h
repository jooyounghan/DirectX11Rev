#pragma once
#include <d3d11.h>

class AViewable
{
public:
	AViewable(
		const float& width,
		const float& height,
		const float& topLeftX = 0.f,
		const float& topLeftY = 0.f,
		const float& minDepth = 0.f,
		const float& maxDepth = 1.f
	);

protected:
	D3D11_VIEWPORT m_viewport;

public:
	inline const D3D11_VIEWPORT& GetViewport() { return m_viewport; }
};

