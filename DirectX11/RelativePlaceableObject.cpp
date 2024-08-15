#include "RelativePlaceableObject.h"
#include "IGuiTopLevelVisitor.h"

using namespace DirectX;

RelativePlaceableObject::RelativePlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: APlaceable(DeviceIn, DeviceContextIn), AAttachable(), AObject()
{
}

RelativePlaceableObject::~RelativePlaceableObject()
{
}

DirectX::XMVECTOR RelativePlaceableObject::GetRotationQuat() const
{
	XMVECTOR ResultQuat = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
	if (ParentObject)
	{
		ResultQuat = XMQuaternionMultiply(ResultQuat, ParentObject->GetRotationQuat());
	}
	return ResultQuat;
}

DirectX::XMMATRIX RelativePlaceableObject::GetScaleMatrix() const
{
	XMMATRIX ResultScale = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	if (ParentObject)
	{
		ResultScale = XMMatrixMultiply(ResultScale, ParentObject->GetScaleMatrix());
	}
	return ResultScale;
}

DirectX::XMMATRIX RelativePlaceableObject::GetRotationMatrix() const
{
	XMMATRIX ResultRotation = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
	if (ParentObject)
	{
		ResultRotation = XMMatrixMultiply(ResultRotation, ParentObject->GetRotationMatrix());
	}
	return ResultRotation;
}

DirectX::XMMATRIX RelativePlaceableObject::GetTranslationMatrix() const
{
	XMMATRIX ResultTranslation = DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
	if (ParentObject)
	{
		ResultTranslation = XMMatrixMultiply(ResultTranslation, ParentObject->GetTranslationMatrix());
	}
	return ResultTranslation;
}

DirectX::XMMATRIX RelativePlaceableObject::GetTransformation(const bool& IsIgnoreScale) const
{
	XMMATRIX ResultTransformation = XMMatrixAffineTransformation(
		IsIgnoreScale ? XMVectorSet(1.f, 1.f, 1.f, 0.0f) : XMVectorSet(Scale.x, Scale.y, Scale.z, 0.0f),
		XMQuaternionIdentity(),
		XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(Angle.Pitch),
			XMConvertToRadians(Angle.Yaw),
			XMConvertToRadians(Angle.Roll)
		),
		Position.Position
	);

	if (ParentObject)
	{
		ResultTransformation = XMMatrixMultiply(
			ResultTransformation, 
			ParentObject->GetTransformation(true)
		);
	}
	return ResultTransformation;
}

void RelativePlaceableObject::UpdateObject(const float& DeltaTimeIn)
{

	TransformationMatrix TempTransformation;

	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);

	for (auto& ChildObject : AttachedObjects)
	{
		ChildObject->UpdateObject(DeltaTimeIn);
	}
}


void RelativePlaceableObject::AcceptGui(IGuiTopLevelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}