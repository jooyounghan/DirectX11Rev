#pragma once
#include "AMeshObject.h"

class StaticMeshAsset;
class PSOObject;

class StaticMeshObject : public AMeshObject
{
public:
	StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn);
	virtual ~StaticMeshObject();

public:
	static std::string StaticMeshObjectKind;

protected:
	std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstance = nullptr;
	MakeSmartPtrSetterGetter(StaticMeshAssetInstance);

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render() override final;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

