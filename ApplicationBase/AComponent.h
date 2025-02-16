#pragma once
#include "IComponentVisitor.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <limits>

#include <vector>
#include <string>
#include <atomic>

class ConstantBuffer;
class DynamicBuffer;

struct SComponent
{
	SComponent(const uint32_t& componentID);
	const uint32_t m_componentID;
	DirectX::XMFLOAT3 dummy;
};

struct STransformation
{
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransformation = DirectX::XMMatrixIdentity();
};

constexpr uint32_t RootID = NULL;
constexpr uint32_t TempID = std::numeric_limits<uint32_t>::max();

class AComponent
{
public:
	AComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const DirectX::XMFLOAT3& localScale	
	);
	virtual ~AComponent();

protected:
	std::string m_componentName;

protected:
	DirectX::XMVECTOR m_localPosition;
	DirectX::XMVECTOR m_localAngle;
	DirectX::XMVECTOR m_localScale;

protected:
	DirectX::XMVECTOR m_absolutePosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteAngle = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteScale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f);

protected:
	STransformation m_transformation;
	DynamicBuffer* m_transformationBuffer;

protected:
	SComponent m_componentConstant;
	DynamicBuffer* m_componentBuffer;

public:
	inline DynamicBuffer* GetTransformationBuffer() const { return m_transformationBuffer; }
	inline DynamicBuffer* GetComponentBuffer() const { return m_componentBuffer; }
	inline const uint32_t& GetComponentID() { return m_componentConstant.m_componentID; }

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
	std::atomic_bool m_isModified = false;

public:
	inline const bool ComsumeIsModified() { return m_isModified.exchange(false, std::memory_order_acquire); }
	virtual void SetIsModified(const bool& isModified);
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
	inline const DirectX::XMVECTOR& GetLocalPosition() const { return m_localPosition; }
	inline const DirectX::XMVECTOR& GetLocalAngle() const { return m_localAngle; }
	inline const DirectX::XMVECTOR& GetLocalScale() const { return m_localScale; }
	inline const DirectX::XMVECTOR& GetAbsolutePosition() const { return m_absolutePosition;}
	inline const DirectX::XMVECTOR& GetAbsoluteAngle() const { return m_absoluteAngle;}
	inline const DirectX::XMVECTOR& GetAbsoluteScale() const { return m_absoluteScale; }
	const DirectX::XMVECTOR GetLocalRotationQuaternion();
	const DirectX::XMVECTOR GetAbsoluteRotationQuaternion() const;

public:
	virtual DirectX::XMMATRIX GetLocalTranformation();
	DirectX::XMMATRIX GetAbsoluteTransformation();

public:
	virtual void UpdateAbsoluteEntities();
	virtual void UpdateComponentTransformation();

public:
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

