#pragma once
#include "LightComponent.h"
#include "ACollidableFrustum.h"
#include "ViewEntity.h"

class SpotLightComponent : public LightComponent, public ACollidableFrustum
{
public:
	SpotLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const DirectX::XMFLOAT3& localAngle,
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd,
		const float& spotPower,
		const uint32_t& lightIdx,
		SLightEntity* lightEntityCached,
		StructuredBuffer* lightEntityCachedBuffer,
		SViewEntity* viewEntityCached,
		StructuredBuffer* viewEntityCachedBuffer,
		const float& fovAngle,
		ID3D11DepthStencilView** depthTestDSVAddressOfCached
	);
	~SpotLightComponent() override = default;

protected:
	float m_fovAngle;
	const D3D11_VIEWPORT m_viewport;
	SViewEntity* m_viewEntityCached = nullptr;
	StructuredBuffer* m_viewEntityBufferCached = nullptr;

public:
	inline const float& GetFovAngle() const { return m_fovAngle; }
	inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
	inline SViewEntity* GetViewEntityAddress() const { return m_viewEntityCached; }
	inline StructuredBuffer* GetViewEntitiesBufferAddress() { return m_viewEntityBufferCached; }

public:
	inline void SetFovAngle(const float& fovAngle) { m_fovAngle = fovAngle; }

protected:
	ID3D11DepthStencilView**  m_depthTestDSVAddressOfCached;

public:
	inline ID3D11DepthStencilView** GetDepthTestDSVCached() const { return m_depthTestDSVAddressOfCached; }

protected:
	DirectX::XMMATRIX m_viewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_projMatrix = DirectX::XMMatrixIdentity();

public:
	const DirectX::XMMATRIX& GetViewMatrix() const { return m_viewMatrix; }
	const DirectX::XMMATRIX& GetProjMatrix() const { return m_projMatrix; }

public:
	void UpdateViewEntity();

public:
	virtual void UpdateEntity() override;
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPSOManager,
		const std::vector<AComponent*>& components
	) override;

public:
	virtual void UpdateBoundingProperty() override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

