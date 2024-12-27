#pragma once
#include "AModelComponent.h"
#include "SkeletalMeshAsset.h"

class SkeletalModelComponent : public AModelComponent
{
public:
	SkeletalModelComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);

protected:
	const SkeletalMeshAsset* m_skeletalMeshAsset;

public:
	void SetSkeletalMeshAsset(const SkeletalMeshAsset* skeletalMeshAsset);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

