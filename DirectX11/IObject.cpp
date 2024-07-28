#include "IObject.h"
#include "DefineUtility.h"

using namespace DirectX;

IObject::IObject()
{
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale = 1.f;
}

DirectX::XMVECTOR IObject::GetRotationQuat() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
}

DirectX::XMMATRIX IObject::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorSet(Scale, Scale, Scale, 0.0f),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		Position.Position
	);
}