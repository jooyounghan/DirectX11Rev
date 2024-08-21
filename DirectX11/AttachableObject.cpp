#include "AttachableObject.h"
#include "PlaceableObject.h"

AttachableObject::AttachableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: AObject(DeviceIn, DeviceContextIn)
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

DirectX::XMMATRIX AttachableObject::GetScaleMatrix() const
{
	XMMATRIX ResultScale = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	if (ParentObject)
	{
		ResultScale = XMMatrixMultiply(ResultScale, ParentObject->GetScaleMatrix());
	}
	return ResultScale;
}

DirectX::XMMATRIX AttachableObject::GetRotationMatrix() const
{
	XMMATRIX ResultRotation = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Angle.Pitch),
		XMConvertToRadians(Angle.Yaw),
		XMConvertToRadians(Angle.Roll)
	);
	if (ParentObject)
	{
		ResultRotation = XMMatrixMultiply(ResultRotation, ParentObject->GetRotationMatrix());
	}
	return ResultRotation;
}

DirectX::XMMATRIX AttachableObject::GetTranslationMatrix() const
{
	XMMATRIX ResultTranslation = DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);
	if (ParentObject)
	{
		ResultTranslation = XMMatrixMultiply(ResultTranslation, ParentObject->GetTranslationMatrix());
	}
	return ResultTranslation;
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
