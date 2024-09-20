#include "APawn.h"
#include "AController.h"

APawn::APawn(MapAsset* MapAssetInstance) : AActor(MapAssetInstance) {}

APawn::~APawn() {}

void APawn::Render()
{
	AActor::Render();
}
