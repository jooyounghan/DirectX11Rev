#pragma once
#include "Actor.h"

class AController;
class InputEventManager;

class APawn : public Actor
{
public:
	APawn(GraphicsPipeline* GraphicsPipelineIn);
	APawn(GraphicsPipeline* GraphicsPipelineIn, std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	virtual ~APawn();

protected:
	std::unique_ptr<AController> Controller;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

