#include "PlacableObject.h"
#include "DefineUtility.h"

using namespace DirectX;

PlacableObject::PlacableObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: IObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{

}

PlacableObject::~PlacableObject()
{
}

void PlacableObject::UpdateObject(const float& DeltaTimeIn, IObject* ParnetObject)
{
	TransformationMatrix TempTransformation;
	TempTransformation.TransfomationMat = GetTransformation();
	TempTransformation.InvTransfomationMat = XMMatrixInverse(nullptr, TempTransformation.TransfomationMat);
	TempTransformation.TransfomationMat = XMMatrixTranspose(TempTransformation.TransfomationMat);

	TransformationBuffer.Upload(DeviceContextCached, TempTransformation);
}
