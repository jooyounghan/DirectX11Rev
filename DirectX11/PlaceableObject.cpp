#include "PlaceableObject.h"

#include "GraphicsPipeline.h"
#include "AttachableObject.h"

using namespace DirectX;

UINT PlaceableObject::PickingIDIssued = 0xABCDEF12;

PlaceableObject::PlaceableObject(GraphicsPipeline* GraphicsPipelineInstance)
	: AObject(GraphicsPipelineInstance), PickingID(PickingIDIssued)
{
	PickingIDIssued++;
	PickingIDBuffer.InitializeForGPU(GraphicsPipelineInstance->GetDevice(), PickingID.GetColor());
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

SPosition4D PlaceableObject::GetAbsolutePosition() const
{
	return Position;
}

SAngle PlaceableObject::GetAbsoluteAngle() const
{
	return Angle;
}

SVector3D PlaceableObject::GetAbsoluteScale() const
{
	return Scale;
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

void PlaceableObject::RemoveAttachedObject(AttachableObject* AttachedObjectIn)
{
	auto it = std::find_if(AttachedChildrenObjects.begin(), AttachedChildrenObjects.end(),
		[AttachedObjectIn](const std::unique_ptr<AttachableObject>& ptr)
		{
			return ptr.get() == AttachedObjectIn;
		}
	);

	if (it != AttachedChildrenObjects.end())
	{
		AttachedChildrenObjects.erase(it);
	}
	else
	{
		for (auto& AtttachedChild : AttachedChildrenObjects)
		{
			AtttachedChild->RemoveAttachedObject(AttachedObjectIn);
		}
	}
}