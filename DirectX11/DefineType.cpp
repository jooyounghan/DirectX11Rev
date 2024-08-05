#include "DefineType.h"
#include <iostream>

void SPosition4D::Normalize()
{
	const float Length = static_cast<float>(sqrt(x * x + y * y + z * z + w * w));
	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}

void SPosition3D::Normalize()
{
	const float Length = static_cast<float>(sqrt(x * x + y * y + z * z));
	x /= Length;
	y /= Length;
	z /= Length;
}

SPosition3D operator-(const SPosition3D& Pos1, const SPosition3D& Pos2)
{
	return SPosition3D{ Pos1.x - Pos2.x, Pos1.y - Pos2.y, Pos1.z - Pos2.z };
}

SPosition3D operator+(const SPosition3D& Pos1, const SPosition3D& Pos2)
{
	return SPosition3D{ Pos1.x + Pos2.x, Pos1.y + Pos2.y, Pos1.z + Pos2.z };
}

SPosition3D operator*(const SPosition3D& Vec, float Scalar)
{
	return SPosition3D{ Vec.x * Scalar, Vec.y * Scalar, Vec.z * Scalar };
}

float InnerProduct(const SPosition3D& Pos1, const SPosition3D& Pos2)
{
	return Pos1.x * Pos2.x + Pos1.y * Pos2.y + Pos1.z * Pos2.z;
}

float InnerProduct(const XMVECTOR& Vec1, const XMVECTOR& Vec2)
{
	return Vec1.m128_f32[0] * Vec2.m128_f32[0] +
		Vec1.m128_f32[1] * Vec2.m128_f32[1] +
		Vec1.m128_f32[2] * Vec2.m128_f32[2] + 
		Vec1.m128_f32[3] * Vec2.m128_f32[3];
}

SCoordinate2D operator-(const SCoordinate2D& Coord1, const SCoordinate2D& Coord2)
{
	return SCoordinate2D{ Coord1.x - Coord2.x, Coord1.y - Coord2.y };
}

SCoordinate2D operator+(const SCoordinate2D& Coord1, const SCoordinate2D& Coord2)
{
	return SCoordinate2D{ Coord1.x + Coord2.x, Coord1.y + Coord2.y };
}

SCoordinate2D operator*(const SCoordinate2D& Vec, float Scalar)
{
	return SCoordinate2D{ Vec.x * Scalar, Vec.y * Scalar };
}

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