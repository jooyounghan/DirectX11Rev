#include "AAttachableObject.h"
#include "APlaceableObject.h"
#include "GlobalVariable.h"

AAttachableObject::AAttachableObject()
	: AObject()
{
}

AAttachableObject::~AAttachableObject()
{
}

void AAttachableObject::SetParent(AObject* ParentObjectIn, ID3D11Buffer* PickingIDBufferCachedIn)
{
	ParentObject = ParentObjectIn;
	PickingIDBufferCached = PickingIDBufferCachedIn;
}

DirectX::XMVECTOR AAttachableObject::GetRotationQuat() const
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

XMFLOAT3 AAttachableObject::GetAbsolutePosition() const
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

XMFLOAT3  AAttachableObject::GetAbsoluteAngle() const
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

XMFLOAT3  AAttachableObject::GetAbsoluteScale() const
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


DirectX::XMMATRIX AAttachableObject::GetTransformation() const
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

void AAttachableObject::MoveForward(const float& DeltaForward)
{
	if (IsIgnoreParentTranslation)
	{
		RelativePosition.z -= DeltaForward;
		return;
	}
	else
	{
		for (auto& AttachedChild : AttachedChildrenObjects)
		{
			AttachedChild->MoveForward(DeltaForward);
		}
	}
}

void AAttachableObject::MoveRight(const float& DeltaRight)
{
	if (IsIgnoreParentTranslation)
	{
		RelativePosition.x -= DeltaRight;
		return;
	}
	else
	{
		for (auto& AttachedChild : AttachedChildrenObjects)
		{
			AttachedChild->MoveRight(DeltaRight);
		}
	}
}

void AAttachableObject::RotatePitch(const float& DeltaPitch)
{
	if (IsIgnoreParentPitch)
	{
		RelativePosition.x -= DeltaPitch;
		return;
	}
	else
	{
		for (auto& AttachedChild : AttachedChildrenObjects)
		{
			AttachedChild->RotatePitch(DeltaPitch);
		}
	}
}

void AAttachableObject::RotateRoll(const float& DeltaRoll)
{
	if (IsIgnoreParentPitch)
	{
		RelativePosition.z -= DeltaRoll;
		return;
	}
	else
	{
		for (auto& AttachedChild : AttachedChildrenObjects)
		{
			AttachedChild->RotateRoll(DeltaRoll);
		}
	}
}

void AAttachableObject::RotateYaw(const float& DeltaYaw)
{
	if (IsIgnoreParentPitch)
	{
		RelativePosition.y -= DeltaYaw;
		return;
	}
	else
	{
		for (auto& AttachedChild : AttachedChildrenObjects)
		{
			AttachedChild->RotateYaw(DeltaYaw);
		}
	}
}

void AAttachableObject::Update(const float& DeltaTimeIn)
{
	AObject::Update(DeltaTimeIn);
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->Update(DeltaTimeIn);
	}
}

void AAttachableObject::AcceptRenderer(ARenderer* Renderer) 
{
	for (auto& ChildObject : AttachedChildrenObjects)
	{
		ChildObject->AcceptRenderer(Renderer);
	}
}

void AAttachableObject::AddIntersectableToRootPlaceable(APlaceableObject* RootPlaceable, IIntersectable* Intersectable)
{
	RootPlaceable->Intersectables.push_back(Intersectable);
}

void AAttachableObject::RemoveAttachedObject(AAttachableObject* AttachedObjectIn)
{
	RemoveFromIntersectables(AttachedObjectIn);

	uint64_t RemovedCount = AttachedChildrenObjects.remove_if(
		[AttachedObjectIn](const std::unique_ptr<AAttachableObject>& ptr)
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

void AAttachableObject::RemoveFromIntersectables(AAttachableObject* AttachedObjectIn)
{
	IIntersectable* IntersectablePtr = dynamic_cast<IIntersectable*>(AttachedObjectIn);
	if (IntersectablePtr != nullptr)
	{
		AObject* CurrentParent = ParentObject;
		AAttachableObject* AttachableParent = nullptr;
		APlaceableObject* PlaceableParent = nullptr;

		AttachableParent = dynamic_cast<AAttachableObject*>(ParentObject);
		while (AttachableParent != nullptr)
		{
			CurrentParent = AttachableParent->ParentObject;
			AttachableParent = dynamic_cast<AAttachableObject*>(AttachableParent->ParentObject);
		}

		PlaceableParent = (APlaceableObject*)CurrentParent;
		PlaceableParent->Intersectables.remove(IntersectablePtr);
	}
}
