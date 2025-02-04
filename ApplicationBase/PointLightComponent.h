#pragma once
#include "AComponent.h"
#include "LightEntity.h"
#include "AViewEntity.h"
#include "CollidableFrustum.h"

#include <wrl/client.h>

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class DSVOption;

class PointLightFrustumPart : public AViewEntity, public CollidableFrustum
{
public:
	PointLightFrustumPart(
		const DirectX::XMFLOAT3& rotationAngle,
		D3D11_VIEWPORT& viewport,
		DirectX::XMVECTOR& absolutePosition,
		float& fallOffEnd
	);
	~PointLightFrustumPart() override = default;

protected:
	const DirectX::XMFLOAT3 m_rotationRadian;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthTestDSV;

protected:
	D3D11_VIEWPORT& m_viewport;
	DirectX::XMVECTOR& m_absolutePosition;
	float& m_fallOffEnd;

public:
	virtual DirectX::XMMATRIX GetViewMatrix() const override;
	virtual DirectX::XMMATRIX GetProjectionMatrix() const override;
	virtual void UpdateViewEntity() override;
	virtual void OnCollide(ICollisionAcceptor*) override;

public:
	inline ID3D11DepthStencilView* GetDepthTestDSV() { return m_depthTestDSV.Get(); }
	inline ID3D11DepthStencilView** GetDepthTestDSVAddress() { return m_depthTestDSV.GetAddressOf(); }
};

class PointLightComponent : public AComponent, public LightEntity
{
public:
	PointLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale,
		const uint32_t& width = GDefaultShadowMapWidth,
		const uint32_t& height = GDefaultShadowMapHeight
	);
	~PointLightComponent() override;

protected:
	D3D11_VIEWPORT m_viewport;
	PointLightFrustumPart m_pointLightFrustumPart[6];

public:
	inline PointLightFrustumPart* GetPointLightFrustumPart(const size_t& idx) { return idx < 6 ? &m_pointLightFrustumPart[idx] : nullptr; }

protected:
	Texture2DInstance<SRVOption>* m_deptTestViewCube = nullptr;

public:
	inline UINT GetWidth() { return static_cast<UINT>(m_viewport.Width); }
	inline UINT GetHeight() { return static_cast<UINT>(m_viewport.Height); }

public:
	void SetDepthTestViewCube(Texture2DInstance<SRVOption>* depthTestViewCube);

public:
	void UpdatePointLightParts();
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) override;
};