#pragma once
#include "APlaceableObject.h"
#include "HeaderHelper.h"

class AMeshAsset;
class MeshObject;

class Actor : public APlaceableObject
{
public:
	Actor(GraphicsPipeline* GraphicsPipelineIn);
	Actor(GraphicsPipeline* GraphicsPipelineIn, std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	virtual ~Actor();

private:
	void Initialize(GraphicsPipeline* GraphicsPipelineIn, std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

protected:
	std::unique_ptr<MeshObject> MeshObjectInstance;
	MakeSmartPtrGetter(MeshObjectInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

