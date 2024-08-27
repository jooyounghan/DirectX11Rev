#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Ray
{
	XMVECTOR	Origin;
	XMVECTOR	Direction;

	static Ray CreateRay(
		const float& ScreenXIn, 
		const float& ScreenYIn, 
		const float& ScreenWidthIn,
		const float& ScreenHeightIn,
		const XMMATRIX& ProjectionMatrix,
		const XMMATRIX& ViewMatrix
	);
};