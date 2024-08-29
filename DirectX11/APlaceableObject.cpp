#include "APlaceableObject.h"

#include "GraphicsPipeline.h"
#include "AAttachableObject.h"

using namespace DirectX;

UINT APlaceableObject::PickingIDIssued = 0xABCDEF12;

APlaceableObject::APlaceableObject(GraphicsPipeline* GraphicsPipelineInstance)
	: AObject(GraphicsPipelineInstance), PickingID(PickingIDIssued)
{
	PickingIDIssued++;
	PickingIDBuffer.InitializeForGPU(GraphicsPipelineInstance->GetDevice(), PickingID.GetColor());
}

APlaceableObject::~APlaceableObject()
{
}

DirectX::XMVECTOR APlaceableObject::GetRotationQuat() const
{
	return XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(RelativeAngle.x),
		XMConvertToRadians(RelativeAngle.y),
		XMConvertToRadians(RelativeAngle.z)
	);
}

XMFLOAT3 APlaceableObject::GetAbsolutePosition() const
{
	return RelativePosition;
}

XMFLOAT3 APlaceableObject::GetAbsoluteAngle() const
{
	return RelativeAngle;
}

XMFLOAT3 APlaceableObject::GetAbsoluteScale() const
{
	return RelativeScale;
}

DirectX::XMMATRIX APlaceableObject::GetTransformation() const
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&RelativeScale),
		XMQuaternionIdentity(),
		GetRotationQuat(),
		XMLoadFloat3(&RelativePosition)
	);
}

void APlaceableObject::Update(const float& DeltaTimeIn)
{	
	AObject::Update(DeltaTimeIn);
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->Update(DeltaTimeIn);
	}
}

void APlaceableObject::AcceptRenderer(ARenderer* Renderer)
{
	for (auto& AtttachedChild : AttachedChildrenObjects)
	{
		AtttachedChild->AcceptRenderer(Renderer);
	}
}

void APlaceableObject::RemoveAttachedObject(AAttachableObject* AttachedObjectIn)
{
	RemoveFromIntersectables(AttachedObjectIn);

	uint64_t RemovedCount = AttachedChildrenObjects.remove_if([AttachedObjectIn](const std::unique_ptr<AAttachableObject>& ptr)
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

void APlaceableObject::RemoveFromIntersectables(AAttachableObject* AttachedObjectIn)
{
	IIntersectable* IntersectablePtr = dynamic_cast<IIntersectable*>(AttachedObjectIn);
	if (IntersectablePtr != nullptr)
	{
		Intersectables.remove(IntersectablePtr);
	}
}
