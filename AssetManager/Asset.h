#pragma once
#include "SerializeHelper.h"
#include "HeaderHelper.h"
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
	bool m_isModified = false;
	MakeGetter(m_assetName, AssetName);
	MakeGetter(m_isModified, IsModified);

public:
	const std::string& GetAssetName() { return m_assetName; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) = 0;

public:
	void SerializeAssetName(
		const AAsset* asset,
		FILE* fileIn
	) const;
};

