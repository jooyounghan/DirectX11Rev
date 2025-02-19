#pragma once
#include "MeshAsset.h"
#include "StaticMeshPartsData.h"

#include <map>

class StaticMeshAsset : public AMeshAsset
{
public:
	StaticMeshAsset() = default;
	StaticMeshAsset(const std::string& assetName);
	~StaticMeshAsset() override;

public:
	const static std::string DefaultSphereModelName;
	const static std::string DefaultCubeModelName;

protected:
	std::map<uint32_t, StaticMeshPartsData*> m_staticMeshPartsPerLOD;

public:
	inline const std::map<uint32_t, StaticMeshPartsData*>& GetStaticMeshPartsPerLOD() { return m_staticMeshPartsPerLOD; }
	
public:
	virtual size_t GetLODCount() const override;
	virtual AMeshPartsData* AddMeshPartData(const uint32_t& lodLevel) override;
	virtual AMeshPartsData* GetMeshPartData(const uint32_t& lodLevel) const override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IStaticMeshProvider
{
public:
	virtual StaticMeshAsset* const GetStaticMeshAsset(
		const std::string& assetName
	) = 0;
};