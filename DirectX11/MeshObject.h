#pragma once
#include "RelativePlaceableObject.h"

#include <memory>

class GraphicsPipeline;
class IMeshAsset;

class MeshObject : public RelativePlaceableObject
{
public:
	MeshObject(
		GraphicsPipeline* GraphicsPipelineInstances, 
		IMeshAsset* MeshAssetInstanceIn
	);
	virtual ~MeshObject();

protected:
	IMeshAsset* MeshAssetInstance = nullptr;
	MakeGetter(MeshAssetInstance);

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;
	virtual void Render(PSOObject* PSOObjectIn) override;
};

