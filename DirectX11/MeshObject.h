#pragma once
#include "AAttachableObject.h"

#include <memory>

class GraphicsPipeline;
class AMeshAsset;

class MeshObject : public AAttachableObject
{
public:
	MeshObject();
	MeshObject(std::shared_ptr<AMeshAsset> MeshAssetInstanceIn);
	virtual ~MeshObject();

private:
	void InitMeshObject();

public:
	static const char* MeshObjectIdentifier;

protected:
	static size_t MeshObjectCount;

protected:
	std::shared_ptr<AMeshAsset> MeshAssetInstance = nullptr;
	MakeSetterGetter(MeshAssetInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void AcceptRenderer(ARenderer* Renderer) override;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

