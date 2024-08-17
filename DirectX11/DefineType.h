#pragma once
#include <DirectXMath.h>

using namespace DirectX;

typedef struct SPosition4D
{
	union
	{
		XMVECTOR Position;
		XMVECTOR Scale;
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			unsigned int ux, uy, uz, uw;
		};
	};
	void Normalize();

	template <typename T>
	int InsertToEmpty(const T& InputElement);
	template<typename T>
	void InsertWithIndex(int IndexIn, const T& InputElement);
} SVector4D;

SPosition4D operator-(const SPosition4D& Pos1, const SPosition4D& Pos2);
SPosition4D operator+(const SPosition4D& Pos1, const SPosition4D& Pos2);
SPosition4D operator*(const SPosition4D& Vec, float Scalar);

template<typename T>
inline int SPosition4D::InsertToEmpty(const T& InputElement)
{
	if (x * x < 1E-9) { memcpy(&x, &InputElement, sizeof(x)); return 0; }
	if (y * y < 1E-9) { memcpy(&y, &InputElement, sizeof(y)); return 1; }
	if (z * z < 1E-9) { memcpy(&z, &InputElement, sizeof(z)); return 2; }
	if (w * w < 1E-9) { memcpy(&w, &InputElement, sizeof(w)); return 3; }
	return -1;
}

template<typename T>
inline void SPosition4D::InsertWithIndex(int IndexIn, const T& InputElement)
{
	switch (IndexIn)
	{
	case 0:
		memcpy(&x, &InputElement, sizeof(x));
		break;
	case 1:
		memcpy(&y, &InputElement, sizeof(y));
		break;
	case 2:
		memcpy(&z, &InputElement, sizeof(z));
		break;
	case 3:
		memcpy(&w, &InputElement, sizeof(w));
		break;
	default:
		break;
	}
}

typedef struct SPosition3D
{
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float Pitch, Roll, Yaw;
		};
	};

	void Normalize();
} SVector3D, SAngle;

SPosition3D operator-(const SPosition3D& Pos1, const SPosition3D& Pos2);
SPosition3D operator+(const SPosition3D& Pos1, const SPosition3D& Pos2);
SPosition3D operator*(const SPosition3D& Vec, float Scalar);

SPosition3D operator*(const SPosition3D& Pos1, const SPosition3D& Pos2);
SPosition3D operator/(const SPosition3D& Pos1, const SPosition3D& Pos2);



float InnerProduct(const SPosition3D& Pos1, const SPosition3D& Pos2);
float InnerProduct(const XMVECTOR& Vec1, const XMVECTOR& Vec2);

struct SCoordinate2D
{
	float x, y;
};

SCoordinate2D operator-(const SCoordinate2D& Coord1, const SCoordinate2D& Coord2);
SCoordinate2D operator+(const SCoordinate2D& Coord1, const SCoordinate2D& Coord2);
SCoordinate2D operator*(const SCoordinate2D& Vec, float Scalar);

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

struct Plane
{
	XMVECTOR Point;
	XMVECTOR Normal;
};