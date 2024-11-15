#pragma once
#include "directxmath/DirectXMath.h"

struct Ray
{
	DirectX::XMVECTOR	Origin;
	DirectX::XMVECTOR	Direction;

	static Ray CreateRay(
		const float& ScreenXIn, 
		const float& ScreenYIn, 
		const float& ScreenWidthIn,
		const float& ScreenHeightIn,
		const DirectX::XMMATRIX& ProjectionMatrix,
		const DirectX::XMMATRIX& ViewMatrix
	);
};