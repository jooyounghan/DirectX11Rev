#pragma once
#include "AAttachableObject.h"

#include <memory>

class GraphicsPipeline;
class AMeshAsset;

class MeshObject : public AAttachableObject
{
public:
	MeshObject(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	virtual ~MeshObject();

public:
	static const char* MeshObjectIdentifier;

protected:
	static size_t MeshObjectCount;

protected:
	std::shared_ptr<AMeshAsset> MeshAssetInstance = nullptr;
	MakeSmartPtrSetterGetter(MeshAssetInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

