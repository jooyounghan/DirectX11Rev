#include "DefineType.h"


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
