#include "APawn.h"
#include "AController.h"

using namespace std;

APawn::APawn(
	MapAsset* MapAssetInstance,
	const string& PlaceableKindIn
) : AActor(MapAssetInstance, PlaceableKindIn) {}

APawn::~APawn() {}

void APawn::Render()
{
	AActor::Render();
}
