#pragma once
#include "AModelComponent.h"
#include "SkeletalMeshAsset.h"

class SkeletalModelComponent : public AModelComponent
{
public:
	SkeletalModelComponent(const SkeletalMeshAsset* const skeletalMeshAsset);

protected:
	const SkeletalMeshAsset* const m_skeletalMeshAsset;

};

