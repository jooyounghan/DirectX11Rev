#pragma once
#include "IComponentVisitor.h"
#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <limits>

#include <vector>
#include <string>
#include <atomic>

struct SComponentEntity
{
	SComponentEntity(
		const uint32_t& componentID, 
		const uint32_t& componentVariable1 = NULL,
		const uint32_t& componentVariable2 = NULL, 
		const uint32_t& componentVariable3 = NULL
	);
	const uint32_t m_componentID;
	const uint32_t m_componentVariable1;
	const uint32_t m_componentVariable2;
	const uint32_t m_componentVariable3;
};

struct STransformationEntity
{
	DirectX::XMMATRIX m_transformation = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invTransposeTransformation = DirectX::XMMatrixIdentity();
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
		const DirectX::XMFLOAT3& scale,
		const uint32_t& componentVariable1 = NULL,
		const uint32_t& componentVariable2 = NULL,
		const uint32_t& componentVariable3 = NULL
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
	virtual const DirectX::XMMATRIX& GetAbsoluteTranformation() const;

public:
	void SetLocalPosition(const DirectX::XMVECTOR& localPosition);
	void SetLocalAngle(const DirectX::XMVECTOR& localAngle);
	void SetScale(const DirectX::XMVECTOR& scale);

protected:
	SComponentEntity m_componentEntity;
	STransformationEntity m_transformationEntity;
	ConstantBuffer m_componentEntityBuffer;
	DynamicBuffer m_transformationEntityBuffer;

public:
	inline DynamicBuffer& GetTransformationEntityBuffer() { return m_transformationEntityBuffer; }
	inline ConstantBuffer& GetComponentEntityBuffer() { return m_componentEntityBuffer; }
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

public:
	inline const bool& IsRenderable() const { return m_isRenderable; }
	inline void SetRenderable(const bool& isRenderable) { m_isRenderable = isRenderable; }
	virtual bool GetDefaultRenderable() const;

protected:
	AtomicFlag m_isDBUpdated = false;
	AtomicFlag m_isTransformationEntityUpdated = false;

public:
	inline AtomicFlag& GetDBUpdatedFlag() { return m_isDBUpdated; }
	inline AtomicFlag& GetTransformationEntityUpdatedFlag() { return m_isTransformationEntityUpdated; }

public:
	void AttachChildComponent(AComponent* component);
	void RemoveFromParent();

public:
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext);
	virtual void Accept(IComponentVisitor* visitor) = 0;
};

