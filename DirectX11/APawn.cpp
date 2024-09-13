#include "APawn.h"
#include "AController.h"
#include "InputEventManager.h"

using namespace std;

APawn::APawn()
	: MeshObjectActor(nullptr)
{
}

APawn::APawn(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn)
	: MeshObjectActor(MeshAssetInstanceIn)
{
}

APawn::~APawn()
{
}

void APawn::Update(const float& DeltaTimeIn)
{
	MeshObjectActor::Update(DeltaTimeIn);
}

void APawn::OnSerializeFromMap(FILE* FileIn)
{
	MeshObjectActor::OnSerializeFromMap(FileIn);
}

void APawn::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	MeshObjectActor::OnDeserializeToMap(FileIn, AssetManagerIn);
}
