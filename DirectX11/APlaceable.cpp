#include "APlaceable.h"
#include "DefineUtility.h"

using namespace DirectX;

APlaceable::APlaceable(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: IObject(), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{
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
