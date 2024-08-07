#include "PlaceableObject.h"
#include "Attachable.h"
#include "IGuiTopLevelVisitor.h"

using namespace DirectX;

PlaceableObject::PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: APlaceable(DeviceIn, DeviceContextIn)
{
}

PlaceableObject::~PlaceableObject()
{
}

DirectX::XMVECTOR PlaceableObject::GetRotationQuat() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
}

DirectX::XMMATRIX PlaceableObject::GetScaleMatrix() const 
{ 
	return DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z); 
}

DirectX::XMMATRIX PlaceableObject::GetRotationMatrix() const 
{ 
	return DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	); 
}

DirectX::XMMATRIX PlaceableObject::GetTranslationMatrix() const 
{ 
	return DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z); 
}

DirectX::XMMATRIX PlaceableObject::GetTransformation(const bool& IsIgnoreScale) const
{
	return XMMatrixAffineTransformation(
		IsIgnoreScale ? XMVectorSet(1.f, 1.f, 1.f, 0.0f) : XMVectorSet(Scale.x, Scale.y, Scale.z, 0.0f),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		Position.Position
	);
}

void PlaceableObject::UpdateObject(const float& DeltaTimeIn)
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

void PlaceableObject::AcceptGui(IGuiTopLevelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}
