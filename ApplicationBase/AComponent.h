#pragma once
#include "IComponentVisitor.h"
#include "ComponentEntity.h"

#include <vector>
#include <string>
#include <atomic>

class AComponent : public ComponentEntity
{
public:
	AComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& relativePosition,
		const DirectX::XMFLOAT3& relativeAngle,
		const DirectX::XMFLOAT3& relativeScale	
	);
	~AComponent() override = default;

protected:
	DirectX::XMVECTOR m_relativePosition;
	DirectX::XMVECTOR m_relativeAngle;
	DirectX::XMVECTOR m_relativeScale;

protected:
	std::string m_componentName;
	std::atomic_bool m_isModified = false;

public:
	inline const std::string& GetComponentName() { return m_componentName; }

public:
	inline const bool ComsumeIsModified() { return m_isModified.exchange(false, std::memory_order_acquire); }
	inline void SetIsModified(const bool& isModified) { m_isModified.store(isModified); }

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;

public:
	inline const std::vector<AComponent*>& GetChildComponents() { return m_childComponents; }
	void AttachChildComponent(AComponent* component);
	void DetachChildComponent(AComponent* component);
	void RemoveFromParent();
	inline AComponent* GetParentComponent() { return m_parentComponent; }

public:
	DirectX::XMVECTOR GetAbsolutePosition();
	DirectX::XMVECTOR GetAbsoluteAngle();
	DirectX::XMVECTOR GetAbsoluteScale();

public:
	inline DirectX::XMVECTOR& GetRelativePositionRef() { return m_relativePosition; }
	inline DirectX::XMVECTOR& GetRelativeAngleRef() { return m_relativeAngle; }
	inline DirectX::XMVECTOR& GetRelativeScaleRef() { return m_relativeScale; }

public:
	virtual void UpdateAbsoluteEntities() override;

public:
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

