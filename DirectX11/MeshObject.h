#pragma once
#include "AttachableObject.h"

#include <memory>

class GraphicsPipeline;
class AMeshAsset;

class MeshObject : public AttachableObject
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
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

};

