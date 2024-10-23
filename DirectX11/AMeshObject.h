#pragma once
#include "AAttachableObject.h"
#include "MaterialAsset.h"

class AMeshAsset;
class MaterialAsset;
class PSOObject;

class AMeshObject : public AAttachableObject
{
public:
	AMeshObject(
		MapAsset* MapAssetInstance,
		const std::string& AttachableKindIn
	);
	virtual ~AMeshObject();

protected:
	std::vector<std::shared_ptr<MaterialAsset>> MaterialAssetInstances;
	MakeGetter(MaterialAssetInstances);

public:
	void SetMaterialAsset(const size_t& MaterialIdx, std::shared_ptr<MaterialAsset> MaterialAssetIn);
	MaterialAsset* GetMaterialAssetInstance(const size_t& MaterialIdx);

public:
	virtual AMeshAsset* GetMeshAssetInstance() = 0;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	size_t GetLODLevel(
		const DirectX::XMFLOAT3& From, 
		const float& MaxDistance, 
		const size_t& MaxLODCount, 
		const size_t SteepLevel
	);

protected:
	PSOObject* MeshObjectPSOCached = nullptr;
	PSOObject* PickingIDSolidPSOCached = nullptr;

public:
	virtual void Render() override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

};

