#pragma once
#include "AModelComponent.h"
#include "StaticMeshAsset.h"

class StaticModelComponent : public AModelComponent
{
public:
	StaticModelComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);

protected:
	const StaticMeshAsset* m_staticMeshAsset;

public:
	void SetStaticMeshAsset(const StaticMeshAsset* staticMeshAsset);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

