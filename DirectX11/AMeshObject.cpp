#include "AMeshObject.h"
#include "AssetManager.h"
#include "AMeshAsset.h"

using namespace std;

AMeshObject::AMeshObject(MapAsset* MapAssetInstance)
	: AAttachableObject(MapAssetInstance)
{
}

AMeshObject::~AMeshObject()
{
}

void AMeshObject::Render()
{
	AAttachableObject::Render();
}
