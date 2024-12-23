#pragma once
#include "AModelComponent.h"
#include "SkeletalMeshAsset.h"

class SkeletalModelComponent : public AModelComponent
{
public:
	SkeletalModelComponent(
		const uint64_t& componentID, DirectX::XMFLOAT3 position,
		const SkeletalMeshAsset* const skeletalMeshAsset
	);

protected:
	const SkeletalMeshAsset* const m_skeletalMeshAsset;

};

