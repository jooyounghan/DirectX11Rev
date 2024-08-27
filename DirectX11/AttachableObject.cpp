#include "AttachableObject.h"
#include "PlaceableObject.h"
#include "GlobalVariable.h"

AttachableObject::AttachableObject(GraphicsPipeline* GraphicsPipelineInstance)
	: AObject(GraphicsPipelineInstance)
{
}

AttachableObject::~AttachableObject()
{
}

DirectX::XMVECTOR AttachableObject::GetRotationQuat() const
{
	XMVECTOR ResultQuat = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(RelativeAngle.x),
		XMConvertToRadians(RelativeAngle.y),
		XMConvertToRadians(RelativeAngle.z)
	);
	if (ParentObject)
	{
		ResultQuat = XMQuaternionMultiply(ResultQuat, ParentObject->GetRotationQuat());
	}
	return ResultQuat;
}

XMFLOAT3 AttachableObject::GetAbsolutePosition() const
{
	XMFLOAT3 AbsPostion = RelativePosition;

	if (ParentObject)
	{
		XMVECTOR Translation;
		XMVECTOR Rotation;
		XMVECTOR Scaling;

		XMVECTOR vRelativePos = XMLoadFloat3(&AbsPostion);

		XMFLOAT3 ParentAbsPos = ParentObject->GetAbsolutePosition();
		XMVECTOR vParentAbsPos = XMLoadFloat3(&ParentAbsPos);

		const XMMATRIX ParentTransformation = ParentObject->GetTransformation();
		XMMatrixDecompose(&Scaling, &Rotation, &Translation, ParentTransformation);

		XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, Rotation);
		XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, Rotation);
		XMVECTOR CurrentRight = XMVector3Rotate(Direction::GDefaultRight, Rotation);
		XMStoreFloat3(&AbsPostion, (CurrentForward * vRelativePos.m128_f32[2] + CurrentUp * vRelativePos.m128_f32[1] + CurrentRight * vRelativePos.m128_f32[0]) + vParentAbsPos);
	}

	return AbsPostion;
}

XMFLOAT3  AttachableObject::GetAbsoluteAngle() const
{
	XMFLOAT3 AbsAngle = RelativeAngle;

	if (ParentObject)
	{
		XMVECTOR vRelativeAngle = XMLoadFloat3(&RelativeAngle);

		XMFLOAT3 ParentAbsAngle = ParentObject->GetAbsoluteAngle();
		XMVECTOR vParentAbsAngle = XMLoadFloat3(&ParentAbsAngle);

		XMStoreFloat3(&AbsAngle, vRelativeAngle + vParentAbsAngle);
	}
	return AbsAngle;
}

XMFLOAT3  AttachableObject::GetAbsoluteScale() const
{
	XMFLOAT3 AbsScale = RelativeScale;
	if (ParentObject)
	{
		XMVECTOR vRelativeScale = XMLoadFloat3(&RelativeScale);

		XMFLOAT3 ParentAbsScale = ParentObject->GetAbsoluteScale();
		XMVECTOR vParentAbsScale = XMLoadFloat3(&ParentAbsScale);

		XMStoreFloat3(&AbsScale, vRelativeScale * vParentAbsScale);
	}
	return AbsScale;
}


DirectX::XMMATRIX AttachableObject::GetTransformation() const
{
	XMMATRIX ResultTransformation = XMMatrixAffineTransformation(
		XMLoadFloat3(&RelativeScale),
		XMQuaternionIdentity(),
		XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(RelativeAngle.x),
			XMConvertToRadians(RelativeAngle.y),
			XMConvertToRadians(RelativeAngle.z)
		),
		XMLoadFloat3(&RelativePosition)
	);

	if (ParentObject)
	{
		ResultTransformation = XMMatrixMultiply(
			ResultTransformation,
			ParentObject->GetTransformation()
		);
	}
	return ResultTransformation;
}

void AttachableObject::UpdateObject(const float& DeltaTimeIn)
{
	AObject::UpdateObject(DeltaTimeIn);
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->UpdateObject(DeltaTimeIn);
	}
}

void AttachableObject::AcceptRenderer(ARenderer* Renderer) 
{
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->AcceptRenderer(Renderer);
	}
}

void AttachableObject::AddIntersectableToRootPlaceable(PlaceableObject* RootPlaceable, IIntersectable* Intersectable)
{
	RootPlaceable->Intersectables.push_back(Intersectable);
}

void AttachableObject::RemoveAttachedObject(AttachableObject* AttachedObjectIn)
{
	RemoveFromIntersectables(AttachedObjectIn);

	uint64_t RemovedCount = AttachedChildrenObjects.remove_if(
		[AttachedObjectIn](const std::unique_ptr<AttachableObject>& ptr)
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

void AttachableObject::RemoveFromIntersectables(AttachableObject* AttachedObjectIn)
{
	IIntersectable* IntersectablePtr = dynamic_cast<IIntersectable*>(AttachedObjectIn);
	if (IntersectablePtr != nullptr)
	{
		AObject* CurrentParent = ParentObject;
		AttachableObject* AttachableParent = nullptr;
		PlaceableObject* PlaceableParent = nullptr;

		AttachableParent = dynamic_cast<AttachableObject*>(ParentObject);
		while (AttachableParent != nullptr)
		{
			CurrentParent = AttachableParent->ParentObject;
			AttachableParent = dynamic_cast<AttachableObject*>(AttachableParent->ParentObject);
		}

		PlaceableParent = (PlaceableObject*)CurrentParent;
		PlaceableParent->Intersectables.remove(IntersectablePtr);
	}
}

