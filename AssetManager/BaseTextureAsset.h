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
		const unsigned int& channelIn,
		uint8_t* imageBufferIn
	);

	virtual ~BaseTextureAsset();

protected:
	unsigned int m_channel;

public:
	virtual std::vector<uint32_t> GetRowPitchArray() override;

public:
	virtual void InitializeGPUAsset(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

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

