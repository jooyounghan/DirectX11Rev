#include "AttachableObject.h"

using namespace DirectX;

AttachableObject::AttachableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: PlaceableObject(DeviceIn, DeviceContextIn)
{
}

AttachableObject::~AttachableObject()
{
}

void AttachableObject::UpdateObject(const float& DeltaTimeIn, IObject* ParentObject)
{
	TransformationMatrix TempTransformation;
	TempTransformation.TransfomationMat = GetTransformation();

	if (ParentObject)
	{
		TempTransformation.TransfomationMat = XMMatrixMultiply(TempTransformation.TransfomationMat, ParentObject->GetTransformation());
	}

	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);

	for (auto& ChildObject : ChildrenObject)
	{
		ChildObject->UpdateObject(DeltaTimeIn, ParentObject);
	}
}
