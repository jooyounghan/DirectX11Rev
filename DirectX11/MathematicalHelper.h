#pragma once
#include "DefineType.h"

class MathematicalHelper
{
public:
	static float InnerProduct(const SVector3D& Vector1, const SVector3D& Vector2);

	static SVector3D Projection(const SVector3D& RefVector, const SVector3D& TargetVector);

	static void GramShmidt(
		const SVector3D& Vector1In,
		const SVector3D& Vector2In,
		const SVector3D& Vector3In,
		SVector3D& Vector2Out,
		SVector3D& Vector3Out
	);

public:
	static void GetTangentBitangent(
		const SPosition3D& Pos0In,
		const SPosition3D& Pos1In,
		const SPosition3D& Pos2In,
		const SCoordinate2D& TexCoord0In,
		const SCoordinate2D& TexCoord1In,
		const SCoordinate2D& TexCoord2In,
		const SVector3D& Normal0In,
		const SVector3D& Normal1In,
		const SVector3D& Normal2In,
		SVector3D& Tangent0Out,
		SVector3D& Tangent1Out,
		SVector3D& Tangent2Out,
		SVector3D& Bitangent0Out,
		SVector3D& Bitangent1Out,
		SVector3D& Bitangent2Out
	);

};

