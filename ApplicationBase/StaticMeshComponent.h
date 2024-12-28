#pragma once
#include "AMeshComponent.h"
#include "StaticMeshAsset.h"

class StaticMeshComponent : public AMeshComponent
{
public:
	StaticMeshComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);

protected:
	std::string m_staticMeshName;
	const StaticMeshAsset* m_staticMeshAsset = nullptr;

public:
	inline void SetStaticMeshName(const std::string& staticMeshName) { m_staticMeshName = staticMeshName; }
	inline const std::string& GetStaticMeshName() { return m_staticMeshName; }

public:
	void UpdateStaticMeshAsset(IStaticMeshProvider& provider);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

