#pragma once
#include "IComponentVisitor.h"
#include "ComponentEntity.h"
#include <vector>
#include <string>

class AComponent : public ComponentEntity
{
public:
	AComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale	
	);

protected:
	std::string m_componentName;
	bool m_isModified = false;

public:
	inline const std::string& GetComponentName() { return m_componentName; }
	inline const bool& GetIsModified() { return m_isModified; }
	inline void SetIsModified(const bool& isModified) { m_isModified = true; }

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;

public:
	inline const std::vector<AComponent*>& GetChildComponents() { return m_childComponents; }
	void AddChildComponent(AComponent* component);
	inline AComponent* GetParentComponent() { return m_parentComponent; }
public:
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

