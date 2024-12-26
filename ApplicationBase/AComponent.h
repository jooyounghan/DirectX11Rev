#pragma once
#include "IComponentVisitor.h"
#include "ComponentEntity.h"
#include <vector>

class AComponent : public ComponentEntity
{
public:
	AComponent(
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale	
	);

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;

public:
	void AddChildComponent(AComponent* component);

public:
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

