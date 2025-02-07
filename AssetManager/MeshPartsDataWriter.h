#pragma once
#include "MeshAsset.h"
#include "IMeshPartsDataVisitor.h"

struct aiMesh;

class MeshPartsDataWriter : public IMeshPartsDataVisitor
{
public:
	MeshPartsDataWriter(
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	);
	~MeshPartsDataWriter() override = default;

protected:
	const bool m_isGltf;
	const aiMesh* const m_mesh;
	const DirectX::XMMATRIX m_transformation;

public:
	virtual void Visit(StaticMeshPartsData* staticMeshPartsData) override;
	virtual void Visit(SkeletalMeshPartsData* skeletalMeshPartsData) override;

public:
	void LoadStaticMeshPartsField(StaticMeshPartsData* meshPartData);
	void LoadSkeletalMeshPartsField(SkeletalMeshPartsData* skeletalMeshPartsData);
};

