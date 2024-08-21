#pragma once
#include "AttachableObject.h"

#include <memory>

class GraphicsPipeline;
class AMeshAsset;

class MeshObject : public AttachableObject
{
public:
	MeshObject(GraphicsPipeline* GraphicsPipelineInstances);
	MeshObject(
		GraphicsPipeline* GraphicsPipelineInstances, 
		AMeshAsset* MeshAssetInstanceIn
	);
	virtual ~MeshObject();

private:
	void InitMeshObject();

public:
	static const char* MeshObjectIdentifier;

protected:
	static size_t MeshObjectCount;

protected:
	AMeshAsset* MeshAssetInstance = nullptr;
	MakeSetterGetter(MeshAssetInstance);

public:
	virtual void UpdateObject(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

