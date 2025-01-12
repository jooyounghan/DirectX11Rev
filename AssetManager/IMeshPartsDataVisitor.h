#pragma once

class MeshPartsData;
class StaticMeshPartsData;
class SkeletalMeshPartsData;

class IMeshPartsDataVisitor
{
public:
	virtual ~IMeshPartsDataVisitor() = default;

public:
	virtual void Visit(StaticMeshPartsData* skeletalMeshAsset) = 0;
	virtual void Visit(SkeletalMeshPartsData* skeletalMeshAsset) = 0;
};

