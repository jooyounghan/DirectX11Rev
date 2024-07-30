#include "PlaceableObject.h"
#include "DefineUtility.h"

using namespace DirectX;

PlaceableObject::PlaceableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: IObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{

}

PlaceableObject::~PlaceableObject()
{
}

void PlaceableObject::UpdateObject(const float& DeltaTimeIn, IObject* ParentObject)
{
	Position.x += 100.f * DeltaTimeIn;
	Angle.Pitch += 90.f * DeltaTimeIn;
	Angle.Roll += 90.f * DeltaTimeIn;
	Angle.Yaw += 90.f * DeltaTimeIn;


	TransformationMatrix TempTransformation;
	TempTransformation.TransfomationMat = XMMatrixIdentity();

	if (ParentObject)
	{
		TempTransformation.TransfomationMat = ParentObject->GetTransformation();
	}
	TempTransformation.TransfomationMat = XMMatrixMultiply(TempTransformation.TransfomationMat, GetTransformation());

	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);

	for (auto& ChildObject : ChildrenObject)
	{
		ChildObject->UpdateObject(DeltaTimeIn, this);
	}
}
