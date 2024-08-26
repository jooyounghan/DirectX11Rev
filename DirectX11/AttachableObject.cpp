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
		XMConvertToRadians(RelativeAngle.Pitch),
		XMConvertToRadians(RelativeAngle.Yaw),
		XMConvertToRadians(RelativeAngle.Roll)
	);
	if (ParentObject)
	{
		ResultQuat = XMQuaternionMultiply(ResultQuat, ParentObject->GetRotationQuat());
	}
	return ResultQuat;
}

SPosition4D AttachableObject::GetAbsolutePosition() const
{
	SPosition4D ResultPostion;

	if (ParentObject)
	{
		XMVECTOR Translation;
		XMVECTOR Rotation;
		XMVECTOR Scaling;

		const XMMATRIX ParentTransformation = ParentObject->GetTransformation();
		XMMatrixDecompose(&Scaling, &Rotation, &Translation, ParentTransformation);

		XMVECTOR CurrentForward = XMVector3Rotate(Direction::GDefaultForward, Rotation);
		XMVECTOR CurrentUp = XMVector3Rotate(Direction::GDefaultUp, Rotation);
		XMVECTOR CurrentRight = XMVector3Rotate(Direction::GDefaultRight, Rotation);

		ResultPostion.Position = CurrentForward * RelativePosition.z + CurrentUp * RelativePosition.y + CurrentRight * RelativePosition.x;

		ResultPostion = ResultPostion + ParentObject->GetAbsolutePosition();
	}
	else
	{
		ResultPostion = RelativePosition;
	}

	return ResultPostion;
}

SAngle AttachableObject::GetAbsoluteAngle() const
{
	SAngle ResultAngle = RelativeAngle;
	if (ParentObject)
	{
		ResultAngle = ResultAngle + ParentObject->GetAbsoluteAngle();
	}
	return ResultAngle;
}

SVector3D AttachableObject::GetAbsoluteScale() const
{
	SVector3D ResultScale = RelativeScale;
	if (ParentObject)
	{
		ResultScale = ResultScale * ParentObject->GetAbsoluteScale();
	}
	return ResultScale;
}


DirectX::XMMATRIX AttachableObject::GetTransformation() const
{
	XMMATRIX ResultTransformation = XMMatrixAffineTransformation(
		XMVectorSet(RelativeScale.x, RelativeScale.y, RelativeScale.z, 0.0f),
		XMQuaternionIdentity(),
		XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(RelativeAngle.Pitch),
			XMConvertToRadians(RelativeAngle.Yaw),
			XMConvertToRadians(RelativeAngle.Roll)
		),
		RelativePosition.Position
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

