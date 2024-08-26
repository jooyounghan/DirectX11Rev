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
		XMConvertToRadians(RelativeAngle.Pitch),
		XMConvertToRadians(RelativeAngle.Yaw),
		XMConvertToRadians(RelativeAngle.Roll)
	);
}

SPosition4D PlaceableObject::GetAbsolutePosition() const
{
	return RelativePosition;
}

SAngle PlaceableObject::GetAbsoluteAngle() const
{
	return RelativeAngle;
}

SVector3D PlaceableObject::GetAbsoluteScale() const
{
	return RelativeScale;
}

DirectX::XMMATRIX PlaceableObject::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMVectorSet(RelativeScale.x, RelativeScale.y, RelativeScale.z, 0.0f),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		RelativePosition.Position
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
	RemoveFromIntersectables(AttachedObjectIn);

	uint64_t RemovedCount = AttachedChildrenObjects.remove_if([AttachedObjectIn](const std::unique_ptr<AttachableObject>& ptr)
		{
			return ptr.get() == AttachedObjectIn;
		}
	);
	
	if (RemovedCount == 0)
	{
		for (auto& AtttachedChild : AttachedChildrenObjects)
		{
			AtttachedChild->RemoveAttachedObject(AttachedObjectIn);
		}
	}
}

void PlaceableObject::RemoveFromIntersectables(AttachableObject* AttachedObjectIn)
{
	IIntersectable* IntersectablePtr = dynamic_cast<IIntersectable*>(AttachedObjectIn);
	if (IntersectablePtr != nullptr)
	{
		Intersectables.remove(IntersectablePtr);
	}
}
