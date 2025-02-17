#pragma once
#include "ATextureAsset.h"

class BaseTextureAsset : public ATextureAsset
{
public:
	BaseTextureAsset() = default;
	BaseTextureAsset(
		const std::string& assetName,
		const unsigned int& widthIn,
		const unsigned int& heightIn,
		uint8_t* imageBufferIn
	);

	~BaseTextureAsset() override = default;

public:
	virtual std::vector<uint32_t> GetRowPitchArray() override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IBaseTextureProvider
{
public:
	virtual BaseTextureAsset* const GetBaseTextureAsset(
		const std::string& assetName
	) = 0;
};

