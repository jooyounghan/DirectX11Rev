#pragma once
#include "AMeshObject.h"

class StaticMeshAsset;
class PSOObject;

class StaticMeshObject : public AMeshObject
{
public:
	StaticMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn);
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
	virtual void Render() override final;

protected:
	virtual std::vector<ID3D11Buffer*> GetMeshObjectVSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectHSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectDSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11Buffer*> GetMeshObjectPSConstants(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectVSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectHSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectDSSRVs(const size_t& MaterialIdx) override;
	virtual std::vector<ID3D11ShaderResourceView*> GetMeshObjectPSSRVs(const size_t& MaterialIdx) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

