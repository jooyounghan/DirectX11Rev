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

public:
	virtual ID3D11Buffer* GetVertexBuffer() = 0;
	virtual UINT GetVertexTypeSize() = 0;
	virtual UINT GetVertexCount() = 0;

public:
	inline ID3D11Buffer* GetIndexBuffer() { return IndicesBuffer.GetBuffer(); }
	inline UINT GetIndexCount() { return static_cast<UINT>(Indices.size()); }
	inline DXGI_FORMAT GetIndexFormat() { return DXGI_FORMAT_R32_UINT; }

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(const std::string& InputFullPath, ID3D11Device* DeviceIn) = 0;
};

