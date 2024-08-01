#include "RelativePlaceableObject.h"

using namespace DirectX;

RelativePlaceableObject::RelativePlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: APlaceable(DeviceIn, DeviceContextIn)
{
}

RelativePlaceableObject::~RelativePlaceableObject()
{
}

void RelativePlaceableObject::UpdateObject(const float& DeltaTimeIn)
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

	for (auto& ChildObject : ChildrenObjects)
	{
		ChildObject->UpdateObject(DeltaTimeIn);
	}
}
