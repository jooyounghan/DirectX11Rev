#include "AObject.h"
#include <format>

using namespace std;

AObject::AObject(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: ObjectID(string{ format("{}", (uint64_t)this) }), DeviceContextCached(DeviceContextIn), TransformationBuffer(DeviceIn)
{
	AutoZeroMemory(Position);
	AutoZeroMemory(Scale);
	AutoZeroMemory(Angle);

	Scale.x = 1.f;
	Scale.y = 1.f;
	Scale.z = 1.f;
}