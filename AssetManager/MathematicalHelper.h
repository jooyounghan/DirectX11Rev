#pragma once
#include <DirectXMath.h>

class MathematicalHelper
{
public:
	static float InnerProduct(const DirectX::XMFLOAT3& Vector1, const DirectX::XMFLOAT3& Vector2);

	static DirectX::XMFLOAT3 Projection(const DirectX::XMFLOAT3& RefVector, const DirectX::XMFLOAT3& TargetVector);

	static void GramShmidt(
		const DirectX::XMFLOAT3& Vector1In,
		const DirectX::XMFLOAT3& Vector2In,
		const DirectX::XMFLOAT3& Vector3In,
		DirectX::XMFLOAT3& Vector2Out,
		DirectX::XMFLOAT3& Vector3Out
	);

public:
	static DirectX::XMFLOAT3 GetTangent(
		const DirectX::XMFLOAT3& Pos0In,
		const DirectX::XMFLOAT3& Pos1In,
		const DirectX::XMFLOAT3& Pos2In,
		const DirectX::XMFLOAT2& TexCoord0In,
		const DirectX::XMFLOAT2& TexCoord1In,
		const DirectX::XMFLOAT2& TexCoord2In,
		const DirectX::XMFLOAT3& NormalIn
	);
};

