#pragma once
#include "SerializeHelper.h"
#include "IAssetVisitor.h"

class AAsset : public ISerializable
{
public:
	AAsset() = default;
	AAsset(const std::string& assetName);
	~AAsset() override = default;

public:
	static std::string AssetExtension;

protected:
	std::string m_assetName;

public:
	inline const std::string& GetAssetName() const { return m_assetName; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
	virtual void Accept(IAssetVisitor* visitor) = 0;
	void SerializeAssetName(const AAsset* asset, FILE* fileIn) const;
};

