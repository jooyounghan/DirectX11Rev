#pragma once
#include "IAssetFile.h"
#include "IndexBuffer.h"

#include <vector>

class IMeshAsset : public IAssetFile
{
public:
	IMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~IMeshAsset();

public:
	virtual void Initialize(ID3D11Device* DeviceIn) = 0;

public:
	std::vector<uint32_t>		Indices;

protected:
	IndexBuffer IndicesBuffer;
	MakeGetter(IndicesBuffer);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) = 0;
};

