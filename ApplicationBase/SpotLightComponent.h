#pragma once
#include "LightComponent.h"
#include "ACollidableFrustum.h"
#include "ViewEntity.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "DSVOption.h"

enum class ESpotLightComponentUpdateOption
{
	VIEW_ENTITY = ELightComponentUpdateOption::LIGHT_COMPONENT_UPDATE_OPTION_OFFSET,
};

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
		SLightEntity* lightEntityCached,
		StructuredBuffer* lightEntityCachedBuffer,
		SViewEntity* viewEntityCached,
		StructuredBuffer* viewEntityCachedBuffer,
		const float& fovAngle
	);
	~SpotLightComponent() override = default;

protected:
	float m_fovAngle;
	const D3D11_VIEWPORT m_viewport;
	SViewEntity* m_viewEntityCached = nullptr;
	StructuredBuffer* m_viewEntityCachedBuffer = nullptr;

public:
	inline const float& GetFovAngle() const { return m_fovAngle; }
	inline SViewEntity* GetViewEntityAddress() const { return m_viewEntityCached; }
	inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
	inline void SetFovAngle(const float& fovAngle) { m_fovAngle = fovAngle; }
	inline void SetViewEntityAddress(SViewEntity* viewEntityCached) { m_viewEntityCached = viewEntityCached; }

protected:
	Texture2DInstance<SRVOption, DSVOption> m_depthTestView;

public:
	inline Texture2DInstance<SRVOption, DSVOption>& GetDepthTestView() { return m_depthTestView; }

protected:
	DirectX::XMMATRIX m_viewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_projMatrix = DirectX::XMMatrixIdentity();

public:
	const DirectX::XMMATRIX& GetViewMatrix() const { return m_viewMatrix; }
	const DirectX::XMMATRIX& GetProjMatrix() const { return m_projMatrix; }

public:
	void UpdateViewEntity();

public:
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

