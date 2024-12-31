#pragma once
#include "MeshAssetWriter.h"

class StaticMeshAssetWriter : public MeshAssetWriter
{
public:
	~StaticMeshAssetWriter() override = default;

public:
	virtual void LoadMeshPartData(
		MeshPartsData* meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	) override;


};

