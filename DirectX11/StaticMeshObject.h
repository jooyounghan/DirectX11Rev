#pragma once
#include "AMeshObject.h"

class StaticMeshAsset;
class PSOObject;

class StaticMeshObject : public AMeshObject
{
public:
	StaticMeshObject(std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn);
	virtual ~StaticMeshObject();

public:
	static std::string StaticMeshObjectKind;

protected:
	std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstance = nullptr;
	MakeSmartPtrGetter(StaticMeshAssetInstance);

public:
	void SetStaticMeshAssetInstance(const std::shared_ptr<StaticMeshAsset>& StaticMeshAssetInstanceIn);

public:
	virtual AMeshAsset* GetMeshAssetInstance() override;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;

protected:
	virtual std::vector<ID3D11Buffer*> GetMeshObjectVSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectHSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectDSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectPSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectVSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectHSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectDSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectPSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx) override;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn) override;
};

