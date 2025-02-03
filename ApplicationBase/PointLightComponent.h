#pragma once
#include "AComponent.h"
#include "LightEntity.h"
#include "IViewEntity.h"
#include "CollidableSphere.h"

#include <wrl/client.h>

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class DSVOption;

class PointLightComponent : public IViewEntity, public AComponent, public LightEntity, public CollidableSphere
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

protected:
	Texture2DInstance<SRVOption>* m_deptTestViewCube = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthTestDSVs[6];

protected:
	SViewEntity m_viewEntities[6];
	DynamicBuffer* m_viewPorjBuffers = nullptr;

public:
	inline UINT GetWidth() { return static_cast<UINT>(m_viewport.Width); }
	inline UINT GetHeight() { return static_cast<UINT>(m_viewport.Height); }
	inline ID3D11DepthStencilView** GetDepthTestDSV(const size_t& idx) { return idx < 6 ? m_depthTestDSVs[idx].GetAddressOf() : nullptr; }

public:
	void SetDepthTestViewCube(Texture2DInstance<SRVOption>* depthTestViewCube);
	DirectX::XMMATRIX GetViewMatrix(const size_t& idx) const;

public:
	virtual DirectX::XMMATRIX GetProjectionMatrix() const override;
	virtual DynamicBuffer* GetViewProjMatrixBuffer() const override;
	virtual void UpdateViewEntity() override;

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};