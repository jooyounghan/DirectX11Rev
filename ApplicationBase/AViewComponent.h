#pragma once
#include "AComponent.h"
#include "CollidableFrustum.h"

#include <d3d11.h>
#include <DirectXMath.h>

template<typename ...IsTextureOption>
class Texture2DInstance;
class RTVOption;
class SRVOption;
class UAVOption;
class DSVOption;
class PureTextureOption;

constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };
constexpr float GDefaultNearZ = 0.1f;
constexpr float GDefaultFarZ = 1E6f;
constexpr float GDefaultFovAngle = 60.f;
constexpr uint32_t GDefaultWidth = 1280;
constexpr uint32_t GDefaultHeight = 960;

struct SViewEntity
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 m_viewPosition = { 0.f, 0.f, 0.f };
	float m_dummy = 0.f;
};

class AViewComponent : public AComponent, public D3D11_VIEWPORT, public CollidableFrustum
{
public:
	AViewComponent(
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
	~AViewComponent() override;

protected:
	float m_nearZ;
	float m_farZ;
	float m_fovAngle;

protected:
	SViewEntity m_viewEntity;
	DynamicBuffer* m_viewProjBuffer;
	static ConstantBuffer* ScreenQuadBuffer;

public:
	inline const float& GetNearZ() { return m_nearZ; }
	inline const float& GetFarZ() { return m_farZ; }
	inline const float& GetFovAngle() { return m_fovAngle; }
	inline DynamicBuffer* GetViewProjMatrixBuffer() const { return m_viewProjBuffer; }

public:
	void SetViewProperties(
		const uint32_t& width, const uint32_t& height,
		const float& nearZ, const float& farZ, const float& fovAngle
	);

public:
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

public:
	virtual void UpdateViewEntity();
	virtual void OnCollide(ICollisionAcceptor*) override;
};

