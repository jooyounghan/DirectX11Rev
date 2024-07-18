#pragma once
#include "Object.h"

class IMeshAsset;
class GraphicsPipeline;
class PSOObject;

class RenderObject : public Object
{
public:
	RenderObject(
		GraphicsPipeline* GraphicsPipelineInstances, 
		IMeshAsset* MeshAssetInstanceIn
	);
	virtual ~RenderObject();

protected:
	IMeshAsset* MeshAssetInstance = nullptr;

public:
	virtual void Render(PSOObject* PSOObjectIn);
};

