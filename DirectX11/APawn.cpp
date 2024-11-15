#include "APawn.h"
#include "AController.h"

using namespace std;

APawn::APawn(const string& PlaceableKindIn)
	: AActor(PlaceableKindIn) {}

APawn::~APawn() {}

void APawn::Render(MapAsset* MapAssetIn)
{
	AActor::Render(MapAssetIn);
}
