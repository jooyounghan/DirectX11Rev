#pragma once
#include "Asset.h"
#include "IBLMaterialAsset.h"
#include "StaticMeshAsset.h"

#include <unordered_map>

class MapAsset : public AAsset
{
public:
	MapAsset() = default;
	MapAsset(const std::string& assetName);
	virtual ~MapAsset();

protected:
	std::string m_iblMaterialName;
	IBLMaterialAsset* m_iblMaterial = nullptr;

protected:
	std::unordered_map<std::string, StaticMeshAsset*> m_assetNameToAssets;
	std::unordered_map<std::string, std::vector<DirectX::XMMATRIX>> m_assetNameToTransformations;

public:
	void UpdateIBLMaterialAsset(
		IIBLMaterialProvider& provider
	);
	void UpdateStaticMeshAssets(
		IStaticMeshProvider& provider
	);

public:
	void SetIBLMaterialAsset(
		const std::string& iblmaterialAssetName,
		IIBLMaterialProvider& provider
	);

	void AddStaticMesh(
		const std::string& assetName, 
		const DirectX::XMMATRIX& transformation,
		IStaticMeshProvider& provider
	);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IMapProvider
{
public:
	virtual MapAsset* const GetMapAsset(
		const std::string& assetName
	) = 0;
};
