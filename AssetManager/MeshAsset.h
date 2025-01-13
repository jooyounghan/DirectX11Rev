#pragma once
#include "Asset.h"
#include "MeshPartsData.h"

#include <vector>


class IModelMaterialProvider;


class AMeshAsset : public AAsset
{
public:
	AMeshAsset() = default;
	AMeshAsset(const std::string& assetName);
	~AMeshAsset() override = default;

protected:
	std::vector<std::string> m_defaultMaterialNames;
	std::vector<ModelMaterialAsset*> m_defaultMaterials;

public:
	inline const std::vector<ModelMaterialAsset*>& GetDefaultMaterials() const { return m_defaultMaterials; }

public:
	void UpdateDefaultMaterialAsset(IModelMaterialProvider& provider);

public:
	virtual size_t GetLODCount() const = 0;
	virtual MeshPartsData* AddMeshPartData(const uint32_t& lodLevel) = 0;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) const = 0;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

