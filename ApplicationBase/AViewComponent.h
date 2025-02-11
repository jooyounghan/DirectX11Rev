#pragma once
#include "AComponent.h"
#include "ACollidableFrustum.h"
#include "AViewEntity.h"

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

class AViewComponent : public AViewEntity, public AComponent, public D3D11_VIEWPORT, public ACollidableFrustum
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
	static ConstantBuffer* ScreenQuadBuffer;

public:
	inline const float& GetFovAngle() { return m_fovAngle; }
	inline void SetFovAngle(const float& fovAngle) { m_fovAngle = fovAngle; }

public:
	void SetViewport(const uint32_t& width, const uint32_t& height);

public:
	virtual DirectX::XMMATRIX GetViewMatrix() const override;

public:
	virtual void UpdateViewEntity() override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

