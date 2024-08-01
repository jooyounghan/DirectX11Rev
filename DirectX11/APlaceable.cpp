#include "APlaceable.h"
#include "DefineUtility.h"

using namespace DirectX;

APlaceable::APlaceable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: IObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale = 1.f;
}

APlaceable::~APlaceable()
{
}

DirectX::XMVECTOR APlaceable::GetRotationQuat() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
}

DirectX::XMMATRIX APlaceable::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorSet(Scale, Scale, Scale, 0.0f),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		Position.Position
	);
}
