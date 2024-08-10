#include "APlaceable.h"
#include "DefineUtility.h"

using namespace DirectX;

uint32_t APlaceable::PickingIDIssued = 0x00000000;

APlaceable::APlaceable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: AObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn), PickingID(PickingIDIssued)
{
	PickingIDIssued++;

	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale.x = 1.f;
	Scale.y = 1.f;
	Scale.z = 1.f;

	PickingIDBuffer.InitializeForGPU(DeviceIn, PickingID.GetColor());
}

APlaceable::~APlaceable()
{
}
