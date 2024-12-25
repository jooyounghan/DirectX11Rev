#pragma once
#include "IComponentVisitor.h"
#include "ComponentEntity.h"
#include <vector>

class AComponent : public ComponentEntity
{
public:
	AComponent(const uint64_t& componentID, DirectX::XMFLOAT3 position);;

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;

public:
	void AddChildComponent(AComponent* component);

public:
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

