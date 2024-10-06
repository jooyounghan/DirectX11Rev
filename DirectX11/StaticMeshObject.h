#pragma once
#include "AMeshObject.h"

class StaticMeshAsset;
class PSOObject;

class StaticMeshObject : public AMeshObject
{
public:
	StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn);
	virtual ~StaticMeshObject();

protected:
	std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstance = nullptr;
	MakeSmartPtrGetter(StaticMeshAssetInstance);

public:
	void SetStaticMeshAssetInstance(std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn);

public:
	static const char* StaticMeshObjectIdentifier;

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

protected:
	PSOObject* StaticMeshObjectPSOCached = nullptr;
	PSOObject* PickingIDSolidStaticPSOCached = nullptr;

public:
	virtual void Render() override final;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

