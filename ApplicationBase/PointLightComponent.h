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

enum class EPointLightComponentUpdateOption
{
	VIEW_ENTITY = ELightComponentUpdateOption::LIGHT_COMPONENT_UPDATE_OPTION_OFFSET,
};

class PointLightFrustum : public ACollidableFrustum
{
public:
	PointLightFrustum(
		const DirectX::XMVECTOR& absolutePositionCached,
		const DirectX::XMVECTOR& absoluteRadianOffset,
		const float& fallOffEndCached,
		SViewEntity* viewEntityCached
	);
	~PointLightFrustum() override = default;

protected:
	const DirectX::XMVECTOR& m_absolutePositionCached;
	const DirectX::XMVECTOR m_absoluteRadianOffset;
	const float& m_fallOffEndCached;

protected:
	SViewEntity* m_viewEntityCached = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthTestDSV;

public:
	inline ID3D11DepthStencilView* GetDepthTestDSV() { return m_depthTestDSV.Get(); }
	inline ID3D11DepthStencilView** GetDepthTestDSVAddress() { return m_depthTestDSV.GetAddressOf(); }
	inline SViewEntity* GetViewEntityAddress() const { return m_viewEntityCached; }
	inline void SetViewEntityAddress(SViewEntity* viewEntityCached) { m_viewEntityCached = viewEntityCached; }

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
		const float& spotPower,
		const uint32_t& lightIndex,
		SLightEntity* lightEntityCached,
		StructuredBuffer* lightEntityCachedBuffer,
		const std::array<SViewEntity*, 6>& viewEntityCached,
		StructuredBuffer* viewCubeEntityCachedBuffer
	);
	~PointLightComponent() override = default;

protected:
	const D3D11_VIEWPORT m_viewport;
	std::array<PointLightFrustum, 6> m_pointLightFrustums;
	StructuredBuffer* m_viewCubeEntityCachedBuffer = nullptr;
	Texture2DInstance<SRVOption> m_deptTestViewCube;

public:
	inline PointLightFrustum& GetPointLightFrustum(const size_t& idx) { return m_pointLightFrustums[idx]; }
	inline Texture2DInstance<SRVOption>& GetDepthTestViewCube() { return m_deptTestViewCube; }

public:
	void UpdatePointLightFrustums();

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) override;
};