#include "APawn.h"
#include "AController.h"

using namespace std;

APawn::APawn(GraphicsPipeline* GraphicsPipelineIn)
	: Actor(GraphicsPipelineIn)
{
}

APawn::APawn(GraphicsPipeline* GraphicsPipelineIn, shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	: Actor(GraphicsPipelineIn, MeshAssetInstanceIn)
{
}

APawn::~APawn()
{
}

void APawn::Update(const float& DeltaTimeIn)
{
	Actor::Update(DeltaTimeIn);
}

void APawn::OnSerialize(FILE* FileIn)
{
	Actor::OnSerialize(FileIn);
}

void APawn::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	Actor::OnDeserialize(FileIn, AssetManagerIn);
}
