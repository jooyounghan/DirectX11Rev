#pragma once
#include "Asset.h"

enum class EAssetType
{
	ASSET_TYPE_NONE,
	ASSET_TYPE_STATIC,
	ASSET_TYPE_SKELETAL,
	ASSET_TYPE_BONE,
	ASSET_TYPE_ANIMATION,
	ASSET_TYPE_BASE_TEXTURE,
	ASSET_TYPE_SCRATCH_TEXTURE,
	ASSET_TYPE_MODEL_MATERIAL,
	ASSET_TYPE_IBL_MATERIAL,
	ASSET_TYPE_RESOURCE,
};

constexpr size_t AssetTypeCount = static_cast<size_t>(EAssetType::ASSET_TYPE_RESOURCE) + 1;

class AssetWrapper : public ISerializable
{
public:
	~AssetWrapper() override = default;

private:
	EAssetType m_assetType = EAssetType::ASSET_TYPE_NONE;

public:
	const EAssetType& GetAssetType() const { return m_assetType; }
	void SetAssetType(const EAssetType& assetType);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};