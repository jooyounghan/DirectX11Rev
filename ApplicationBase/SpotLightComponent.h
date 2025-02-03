#pragma once
#include "AViewComponent.h"
#include "CollidableFrustum.h"
#include "LightEntity.h"

class DepthTestRenderer;

class SpotLightComponent : public AViewComponent, public LightEntity
{
public:
	SpotLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale,
		const float& fovAngle = GDefaultFovAngle,
		const float& farZ = GDefaultFarZ
	);
	~SpotLightComponent() override;

protected:
	Texture2DInstance<SRVOption, DSVOption>* m_depthTestView = nullptr;

public:
	void SetDepthTestView(Texture2DInstance<SRVOption, DSVOption>* depthTestView);

public:
	inline const Texture2DInstance<SRVOption, DSVOption>* GetDepthTestView() { return m_depthTestView; }

public:
	virtual DirectX::XMMATRIX GetProjectionMatrix() const override;
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void GenerateShadowMap(
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		const std::vector<AComponent*>& components
	) override;

private:
	void DepthTestImpl(AComponent* component, DepthTestRenderer& depthTestRenderer);
};

