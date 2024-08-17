#include "PlaceableObject.h"
#include "AttachableObject.h"

using namespace DirectX;

UINT PlaceableObject::PickingIDIssued = 0xABCDEF12;

PlaceableObject::PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: AObject(DeviceIn, DeviceContextIn), PickingID(PickingIDIssued)
{
	PickingIDIssued++;
	PickingIDBuffer.InitializeForGPU(DeviceIn, PickingID.GetColor());
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

DirectX::XMMATRIX PlaceableObject::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorSet(Scale.x, Scale.y, Scale.z, 0.0f),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		Position.Position
	);
}

void PlaceableObject::UpdateObject(const float& DeltaTimeIn)
{	
	AObject::UpdateObject(DeltaTimeIn);
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->UpdateObject(DeltaTimeIn);
	}
}

void PlaceableObject::AcceptRenderer(ARenderer* Renderer)
{
	for (auto& AtttachedChild : AttachedChildrenObjects)
	{
		AtttachedChild->AcceptRenderer(Renderer);
	}

}