#pragma once
#include "PlaceableObject.h"
#include "IRenderable.h"
#include "HeaderHelper.h"

#include <memory>

class IMeshAsset;
class GraphicsPipeline;
class IIntersectable;

class RenderObject : public PlaceableObject, public IRenderable
{
public:
	RenderObject(
		GraphicsPipeline* GraphicsPipelineInstances, 
		IMeshAsset* MeshAssetInstanceIn
	);
	virtual ~RenderObject();

protected:
	IMeshAsset* MeshAssetInstance = nullptr;
	MakeGetter(MeshAssetInstance);

protected:
	std::unique_ptr<IIntersectable> InterstectInstance = nullptr;
	MakeSmartPtrGetter(InterstectInstance);


public:
	virtual void Render(PSOObject* PSOObjectIn) override;
};

