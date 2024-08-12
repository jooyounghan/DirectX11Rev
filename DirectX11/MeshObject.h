#pragma once
#include "RelativePlaceableObject.h"

#include <memory>

class GraphicsPipeline;
class AMeshAsset;

class MeshObject : public RelativePlaceableObject
{
public:
	MeshObject(
		GraphicsPipeline* GraphicsPipelineInstances, 
		AMeshAsset* MeshAssetInstanceIn
	);
	virtual ~MeshObject();

protected:
	AMeshAsset* MeshAssetInstance = nullptr;
	MakeGetter(MeshAssetInstance);

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiLowLevelVisitor* GuiVisitor) override;

};

