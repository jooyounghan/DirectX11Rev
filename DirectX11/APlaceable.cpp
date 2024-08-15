#include "APlaceable.h"
#include "DefineUtility.h"

using namespace DirectX;

UINT APlaceable::PickingIDIssued = 0xABCDEF12;

APlaceable::APlaceable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: AObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn), PickingID(PickingIDIssued)
{

	PickingIDIssued++;
	PickingIDBuffer.InitializeForGPU(DeviceIn, PickingID.GetColor());
	
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale.x = 1.f;
	Scale.y = 1.f;
	Scale.z = 1.f;
}

APlaceable::~APlaceable()
{
}
