#include "DefineType.h"
#include <iostream>

Ray Ray::CreateRay(
	const float& ScreenXIn, 
	const float& ScreenYIn, 
	const float& ScreenWidthIn,
	const float& ScreenHeightIn,
	const XMMATRIX& ProjectionMatrix,
	const XMMATRIX& ViewMatrix
)
{
	XMVECTOR NearPoint = XMVectorSet(
		(2.0f * ScreenXIn / ScreenWidthIn) - 1.0f,
		1.0f - (2.0f * ScreenYIn / ScreenHeightIn),
		0.f, 1.f
	);

	XMVECTOR FarPoint = XMVectorSet(
		(2.0f * ScreenXIn / ScreenWidthIn) - 1.0f,
		1.0f - (2.0f * ScreenYIn / ScreenHeightIn),
		1.f, 1.f
	);

	const XMMATRIX ProjInvMatrix = XMMatrixInverse(nullptr, ProjectionMatrix);
	NearPoint = XMVector3TransformCoord(NearPoint, ProjInvMatrix);
	FarPoint = XMVector3TransformCoord(FarPoint, ProjInvMatrix);

	const XMMATRIX ViewInvMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	NearPoint = XMVector3TransformCoord(NearPoint, ViewInvMatrix);
	FarPoint = XMVector3TransformCoord(FarPoint, ViewInvMatrix);

	const XMVECTOR Displacement = FarPoint - NearPoint;
	const XMVECTOR Direction = XMVector3Normalize(Displacement);

	return Ray{ NearPoint, Direction };
}