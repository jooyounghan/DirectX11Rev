#include "MathematicalHelper.h"

float MathematicalHelper::InnerProduct(const SVector3D& Vector1, const SVector3D& Vector2)
{
	return Vector1.x * Vector2.x + Vector1.y * Vector2.y + Vector1.z * Vector2.z;
}

SVector3D MathematicalHelper::Projection(const SVector3D& RefVector, const SVector3D& TargetVector)
{
	return (RefVector * (InnerProduct(RefVector, TargetVector) / InnerProduct(RefVector, RefVector)));
}

void MathematicalHelper::GramShmidt(
	const SVector3D& Vector1In, 
	const SVector3D& Vector2In, 
	const SVector3D& Vector3In, 
	SVector3D& Vector2Out, 
	SVector3D& Vector3Out
)
{
	Vector2Out = Vector2In - Projection(Vector1In, Vector2In);
	Vector3Out = Vector3In - Projection(Vector1In, Vector3In) - Projection(Vector2Out, Vector3In);
}

void MathematicalHelper::GetTangentBitangent(
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
)
{
	SPosition3D DeltaPos1 = Pos1In - Pos0In;
    SPosition3D DeltaPos2 = Pos2In - Pos0In;

    // UV 벡터
    SCoordinate2D DeltaUV1 = TexCoord1In - TexCoord0In;
    SCoordinate2D DeltaUV2 = TexCoord2In - TexCoord0In;

    // 행렬의 요소 계산
    float Determinant = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV1.y * DeltaUV2.x);

    SVector3D Tangent = (DeltaPos1 * DeltaUV2.y - DeltaPos2 * DeltaUV1.y) * Determinant;
    SVector3D Bitangent = (DeltaPos2 * DeltaUV1.x - DeltaPos1 * DeltaUV2.x) * -Determinant;

	GramShmidt(
		Normal0In,
		Tangent,
		Bitangent,
		Tangent0Out,
		Bitangent0Out
	);
	GramShmidt(
		Normal1In,
		Tangent,
		Bitangent,
		Tangent1Out,
		Bitangent1Out
	);
	GramShmidt(
		Normal2In,
		Tangent,
		Bitangent,
		Tangent2Out,
		Bitangent2Out
	);

	Tangent0Out.Normalize();
	Tangent1Out.Normalize();
	Tangent2Out.Normalize();
	Bitangent0Out.Normalize();
	Bitangent1Out.Normalize();
	Bitangent2Out.Normalize();
}

