#include "AttachableObject.h"
#include "PlaceableObject.h"

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

SPosition4D AttachableObject::GetAbsolutePosition() const
{
	SPosition4D ResultPostion = Position;
	if (ParentObject)
	{
		ResultPostion = ResultPostion + ParentObject->GetAbsolutePosition();
	}
	return ResultPostion;
}

SAngle AttachableObject::GetAbsoluteAngle() const
{
	SAngle ResultAngle = Angle;
	if (ParentObject)
	{
		ResultAngle = ResultAngle + ParentObject->GetAbsoluteAngle();
	}
	return ResultAngle;
}

SVector3D AttachableObject::GetAbsoluteScale() const
{
	SVector3D ResultScale = Scale;
	if (ParentObject)
	{
		ResultScale = ResultScale * ParentObject->GetAbsoluteScale();
	}
	return ResultScale;
}


DirectX::XMMATRIX AttachableObject::GetTransformation() const
{
	XMMATRIX ResultTransformation = XMMatrixAffineTransformation(
		XMVectorSet(Scale.x, Scale.y, Scale.z, 0.0f),
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
