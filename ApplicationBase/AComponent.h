#pragma once
#include "IComponentVisitor.h"
#include "DynamicBuffer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <limits>

#include <vector>
#include <string>
#include <atomic>

struct SComponentEntity
{
	SComponentEntity(const uint32_t& componentID);
	const uint32_t m_componentID;
	DirectX::XMFLOAT3 dummy;
};

struct STransformationEntity
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
		const DirectX::XMFLOAT3& scale
	);
	virtual ~AComponent() = default;

protected:
	std::string m_componentName;
	DirectX::XMVECTOR m_localPosition;
	DirectX::XMVECTOR m_localAngle;
	DirectX::XMVECTOR m_absolutePosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_absoluteAngle = DirectX::XMVectorZero();
	DirectX::XMVECTOR m_scale;
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();

public:
	inline const std::string& GetComponentName() const { return m_componentName; }
	inline const DirectX::XMVECTOR& GetLocalPosition() const { return m_localPosition; }
	inline const DirectX::XMVECTOR& GetLocalAngle() const { return m_localAngle; }
	inline const DirectX::XMVECTOR& GetAbsolutePosition() const { return m_absolutePosition; }
	inline const DirectX::XMVECTOR& GetAbsoluteAngle() const { return m_absoluteAngle; }
	inline const DirectX::XMVECTOR& GetScale() const { return m_scale; }
	DirectX::XMVECTOR GetLocalRotationQuaternion() const;
	DirectX::XMVECTOR GetAbsoluteRotationQuaternion() const;
	virtual DirectX::XMMATRIX GetLocalTransformation() const;
	inline const DirectX::XMMATRIX& GetAbsoluteTranformation() const { return m_transformation; }

protected:
	STransformationEntity m_transformationEntity;
	SComponentEntity m_componentEntity;
	DynamicBuffer m_transformationEntityBuffer;
	DynamicBuffer m_componentEntityBuffer;

public:
	inline DynamicBuffer& GetTransformationEntityBuffer() { return m_transformationEntityBuffer; }
	inline DynamicBuffer& GetComponentEntityBuffer() { return m_componentEntityBuffer; }
	inline const uint32_t& GetComponentID() const { return m_componentEntity.m_componentID; }

protected:
	std::vector<AComponent*> m_childComponents;
	AComponent* m_parentComponent = nullptr;
	uint32_t m_parentSceneID = NULL;

public:
	inline const std::vector<AComponent*>& GetChildComponents() const { return m_childComponents; }
	inline AComponent* GetParentComponent() const { return m_parentComponent; }
	inline const uint32_t& GetParentSceneID() const { return m_parentSceneID; }
	void SetParentSceneID(const uint32_t& parentSceneID) { m_parentSceneID = parentSceneID; }

protected:
	bool m_isRenderable = true;
	std::atomic_bool m_isModified = false;

public:
	inline const bool ComsumeIsModified() { return m_isModified.exchange(false, std::memory_order_acquire); }
	inline const bool& IsRenderable() const { return m_isRenderable; }
	inline void SetRenderable(const bool& isRenderable) { m_isRenderable = isRenderable; }

public:
	virtual bool GetDefaultRenderable() const;

public:
	void AttachChildComponent(AComponent* component);
	void RemoveFromParent();

public:
	virtual void UpdateEntity();
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

