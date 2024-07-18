#include "Object.h"
#include "DefineUtility.h"

using namespace DirectX;

Object::Object(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale.x = Scale.y = Scale.z = 1.f;
}

Object::~Object()
{
}

DirectX::XMVECTOR Object::GetRotationQuat()
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
}

DirectX::XMMATRIX Object::GetTransformation()
{
	return XMMatrixAffineTransformation(
		Scale.Scale, 
		XMQuaternionIdentity(), 
		GetRotationQuat(),
		Position.Position
	);
}

void Object::UpdateObject()
{
	Angle.Pitch += 0.1f;
	Angle.Roll += 0.15f;
	Angle.Yaw += 0.2f;

	TransformationMatrix TempTransformation;
	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);
}
