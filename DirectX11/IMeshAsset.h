#pragma once
#include "IAssetFile.h"
#include "IIndexable.h"

class IMeshAsset : public IAssetFile, public IIndexable<uint32_t>
{
public:
	IMeshAsset(const std::string& AssetNameIn, EAssetType AssetTypeIn);
	virtual ~IMeshAsset();

public:
	virtual DXGI_FORMAT GetIndexFormat() override { return DXGI_FORMAT_R32_UINT; }

public:
	virtual void GetVertexInformation(ID3D11Buffer*& RefVertexBuffer, UINT& RefVertexTypeSize) = 0;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) = 0;
};

