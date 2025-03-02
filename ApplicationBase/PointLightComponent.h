#pragma once
#include "LightComponent.h"
#include "ACollidableFrustum.h"
#include "ViewEntity.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"

#include <array>
#include <wrl/client.h>

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class DSVOption;

class PointLightFrustum : public ACollidableFrustum
{
public:
	PointLightFrustum(
		const DirectX::XMVECTOR& absolutePositionCached,
		const DirectX::XMVECTOR& absoluteRadianOffset,
		const float& fallOffEndCached,
		ID3D11DepthStencilView** depthTestDSVAddressOfCached
	);
	~PointLightFrustum() override = default;

protected:
	const DirectX::XMVECTOR& m_absolutePositionCached;
	const DirectX::XMVECTOR m_absoluteRadianOffset;
	const float& m_fallOffEndCached;

protected:
	SViewEntity m_viewEntity;
	DynamicBuffer m_viewEntityBuffer;
	ID3D11DepthStencilView** m_depthTestDSVAddressOfCached;

public:
	DynamicBuffer& GetViewEntityBuffer() { return m_viewEntityBuffer; }
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
	virtual void UpdateBoundingProperty() override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

class PointLightComponent : public LightComponent
{
public:
	PointLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& localPosition,
		const float& lightPower,
		const float& fallOffStart,
		const float& fallOffEnd,
		const uint32_t& lightIdx,
		SLightEntity* lightEntityCached,
		StructuredBuffer* lightEntityCachedBuffer, 
		DirectX::XMVECTOR* lightPositionCached,
		StructuredBuffer* lightPositionsBufferCached,
		const std::array<ID3D11DepthStencilView**, 6>& depthTestDSVCubeAddressOfCached
	);
	~PointLightComponent() override = default;

protected:
	const D3D11_VIEWPORT m_viewport;
	std::array<PointLightFrustum, 6> m_pointLightFrustums;
	
protected:
	DirectX::XMVECTOR* m_lightPositionCached = nullptr;
	StructuredBuffer* m_lightPositionsBufferCached = nullptr;

public:
	inline PointLightFrustum& GetPointLightFrustum(const size_t& idx) { return m_pointLightFrustums[idx]; }

public:

	void UpdatePointLightFrustums();

public:
	virtual void UpdateEntity() override;
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) override;
};