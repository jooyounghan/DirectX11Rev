#pragma once
#include "IComponentVisitor.h"
#include "ComponentEntity.h"
#include "AModifiable.h"

#include <vector>
#include <string>
#include <atomic>

class AComponent : public ComponentEntity, public AModifiable
{
public:
	AComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& localScale	
	);
	~AComponent() override = default;

protected:
	std::string m_componentName;

protected:
	DirectX::XMVECTOR m_absolutePosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteAngle = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteScale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f);

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;

protected:
	uint32_t m_parentSceneID = NULL;
	
public:
	inline const uint32_t& GetParentSceneID() { return m_parentSceneID; }
	void SetParentSceneID(const uint32_t& parentSceneID) { m_parentSceneID = parentSceneID; }

protected:
	bool m_isRenderable = true;

public:
	inline const bool& IsRenderable() { return m_isRenderable; }
	inline void SetRenderable(const bool& isRenderable) { m_isRenderable = isRenderable; }

public:
	virtual bool GetDefaultRenderable();

public:
	void AttachChildComponent(AComponent* component);
	void RemoveFromParent();

public:
	inline const std::string& GetComponentName() const { return m_componentName; }
	inline const std::vector<AComponent*>& GetChildComponents() const { return m_childComponents; }
	inline AComponent* GetParentComponent() const { return m_parentComponent; }
	inline const DirectX::XMVECTOR& GetAbsolutePosition() const { return m_absolutePosition;}
	inline const DirectX::XMVECTOR& GetAbsoluteAngle() const { return m_absoluteAngle;}
	inline const DirectX::XMVECTOR& GetAbsoluteScale() const { return m_absoluteScale; }
	const DirectX::XMVECTOR GetAbsoluteRotationQuaternion() const;

public:
	DirectX::XMMATRIX GetAbsoluteTransformation();

public:
	virtual void UpdateAbsoluteEntities();
	virtual void UpdateComponentTransformation();

public:
	virtual void SetIsModified(const bool& isModified) override;
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

