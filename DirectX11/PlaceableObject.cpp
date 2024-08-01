#include "PlaceableObject.h"
#include "Attachable.h"

PlaceableObject::PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: APlaceable(DeviceIn, DeviceContextIn)
{
}

PlaceableObject::~PlaceableObject()
{
}

void PlaceableObject::UpdateObject(const float& DeltaTimeIn)
{	
	TransformationMatrix TempTransformation;

	TempTransformation.TransfomationMat = GetTransformation();	
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);
	
	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);
	
	for (auto& ChildObject : ChildrenObjects)
	{
		ChildObject->UpdateObject(DeltaTimeIn);
	}
}
