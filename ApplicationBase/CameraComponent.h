#pragma once
#include "AComponent.h"

template<typename ...IsTextureOption>
class Texture2DInstance;
class RTVOption;
class SRVOption;
class UAVOption;
class DSVOption;

constexpr DirectX::XMVECTOR GDefaultForward = DirectX::XMVECTOR{ 0.f, 0.f, 1.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultUp = DirectX::XMVECTOR{ 0.f, 1.f, 0.f, 0.f };
constexpr DirectX::XMVECTOR GDefaultRight = DirectX::XMVECTOR{ 1.f, 0.f, 0.f, 0.f };
constexpr float GDefaultNearZ = 0.01f;
constexpr float GDefaultFarZ = 1E6f;
constexpr float GDefaultFovAngle = 60.f;

struct SViewElement
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
};

class CameraComponent : public AComponent, public D3D11_VIEWPORT
{
public:
	CameraComponent(
		const std::string& componentName,
		const uint32_t& componentID, 
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale
	);
	~CameraComponent() override;

protected:
	float m_nearZ;
	float m_farZ;
	float m_fovAngle;

public:
	inline const float& GetNearZ() { return m_nearZ; }
	inline const float& GetFarZ() { return m_farZ; }
	inline const float& GetFovAngle() { return m_fovAngle; }

public:
	void SetCameraProperties(
		const uint32_t& width, const uint32_t& height,
		const float& nearZ, const float& farZ, const float& fovAngle
	);

protected:
	SViewElement m_viewElement;
	DynamicBuffer* m_viewProjBuffer;

public:
	const DynamicBuffer* GetViewProjMatrixBuffer() const { return m_viewProjBuffer; }

public:
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

protected:
	Texture2DInstance<SRVOption, RTVOption, UAVOption>* m_film = nullptr;
	Texture2DInstance<DSVOption>* m_depthStencilViewBuffer = nullptr;

public:
	const Texture2DInstance<SRVOption, RTVOption, UAVOption>* const GetFilm() { return m_film; }
	const Texture2DInstance<DSVOption>* const GetDepthStencilViewBuffer() { return m_depthStencilViewBuffer; }

public:
	virtual void InitEntity(ID3D11Device* device) override;
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext) override;

public:
	virtual void Accept(IComponentVisitor* visitor) override;
};

