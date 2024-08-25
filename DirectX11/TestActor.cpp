#include "TestActor.h"
#include "GraphicsPipeline.h"

using namespace std;

TestActor::TestActor(GraphicsPipeline* GraphicsPipelineIn)
	: PlaceableObject(GraphicsPipelineIn)
{
	static size_t TestActorCount = 0;
	TestActorCount++;
	ObjectName = "Test Actor" + to_string(TestActorCount);

	PlaceableKind = EPlaceableObjectKind::ActorKind;
}

TestActor::~TestActor()
{
}

void TestActor::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);
}

void TestActor::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserialize(FileIn, AssetManagerIn);
}
