#include "APawn.h"
#include "AController.h"
#include "InputEventManager.h"

using namespace std;

APawn::APawn()
	: Actor()
{
}

APawn::APawn(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	: Actor(MeshAssetInstanceIn)
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
