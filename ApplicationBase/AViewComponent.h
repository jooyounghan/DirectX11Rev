#pragma once
#include "AComponent.h"
#include "CollidableFrustum.h"
#include "IViewEntity.h"

template<typename ...IsTextureOption>
class Texture2DInstance;
class RTVOption;
class SRVOption;
class UAVOption;
class DSVOption;
class PureTextureOption;

constexpr float GDefaultNearZ = 0.1f;
constexpr float GDefaultFarZ = 1E6f;
constexpr float GDefaultFovAngle = 60.f;
constexpr uint32_t GDefaultViewWidth = 1280;
constexpr uint32_t GDefaultViewHeight = 960;

class AViewComponent : public IViewEntity, public AComponent, public D3D11_VIEWPORT, public CollidableFrustum
{
public:
	AViewComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale,
		const uint32_t& width = GDefaultViewWidth,
		const uint32_t& height = GDefaultViewHeight,
		const float& fovAngle = GDefaultFovAngle
	);
	~AViewComponent() override;

protected:
	float m_fovAngle;

protected:
	SViewEntity m_viewEntity;
	DynamicBuffer* m_viewProjBuffer = nullptr;
	static ConstantBuffer* ScreenQuadBuffer;

public:
	inline const float& GetFovAngle() { return m_fovAngle; }
	inline void SetFovAngle(const float& fovAngle) { m_fovAngle = fovAngle; }

public:
	virtual DynamicBuffer* GetViewProjMatrixBuffer() const override;

public:
	void SetViewport(const uint32_t& width, const uint32_t& height);

public:
	DirectX::XMMATRIX GetViewMatrix() const;

public:
	virtual void UpdateViewEntity() override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

