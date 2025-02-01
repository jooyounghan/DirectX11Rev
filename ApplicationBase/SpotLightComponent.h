#pragma once
#include "AViewComponent.h"
#include "CollidableFrustum.h"
#include "LightEntity.h"

class SpotLightComponent : public AViewComponent, public LightEntity
{
public:
	SpotLightComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale,
		const uint32_t& width = GDefaultWidth,
		const uint32_t& height = GDefaultHeight,
		const float& nearZ = GDefaultNearZ,
		const float& farZ = GDefaultFarZ,
		const float& fovAngle = GDefaultFovAngle
	);
	~SpotLightComponent() override;

protected:
	Texture2DInstance<SRVOption, DSVOption>* m_depthTestView = nullptr;

public:
	void SetDepthTestView(Texture2DInstance<SRVOption, DSVOption>* depthTestView);

public:
	inline const Texture2DInstance<SRVOption, DSVOption>* GetDepthTestView() { return m_depthTestView; }

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

