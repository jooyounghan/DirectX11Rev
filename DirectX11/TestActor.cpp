#include "TestActor.h"

using namespace std;

TestActor::TestActor(ID3D11Device* DeviceIn, ID3D11DeviceContext* DeviceContextIn)
	: PlaceableObject(DeviceIn, DeviceContextIn)
{
	static size_t TestActorCount = 0;
	TestActorCount++;
	ObjectName = "Test Actor" + to_string(TestActorCount);
}

TestActor::~TestActor()
{
}
