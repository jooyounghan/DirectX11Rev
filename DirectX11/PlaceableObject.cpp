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
		XMConvertToRadians(RelativeAngle.x),
		XMConvertToRadians(RelativeAngle.y),
		XMConvertToRadians(RelativeAngle.z)
	);
}

XMFLOAT3 PlaceableObject::GetAbsolutePosition() const
{
	return RelativePosition;
}

XMFLOAT3 PlaceableObject::GetAbsoluteAngle() const
{
	return RelativeAngle;
}

XMFLOAT3 PlaceableObject::GetAbsoluteScale() const
{
	return RelativeScale;
}

DirectX::XMMATRIX PlaceableObject::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&RelativeScale),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		XMLoadFloat3(&RelativePosition)
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
