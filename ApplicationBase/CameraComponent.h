#pragma once
#include "AComponent.h"
#include "CollidableFrustum.h"

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


struct SViewElement
{
	DirectX::XMMATRIX m_viewProj = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_invViewProj = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 m_viewPosition;
	float m_dummy;
};

class CameraComponent : public AComponent, public D3D11_VIEWPORT, public CollidableFrustum
{
public:
	CameraComponent(
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
	static ConstantBuffer* ScreenQuadBuffer;

public:
	DynamicBuffer* GetViewProjMatrixBuffer() const { return m_viewProjBuffer; }

public:
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

protected:
	Texture2DInstance<SRVOption, RTVOption, UAVOption>* m_film = nullptr;
	Texture2DInstance<RTVOption>*			m_idFilm = nullptr;
	Texture2DInstance<PureTextureOption>*	m_idStagingFilm = nullptr;
	Texture2DInstance<DSVOption>*			m_depthStencilViewBuffer = nullptr;

public:
	void SetFilm(Texture2DInstance<SRVOption, RTVOption, UAVOption>* film);
	void SetIDFilm(Texture2DInstance<RTVOption>* idFilm);
	void SetIDStagingFilm(Texture2DInstance<PureTextureOption>* idStagingFilm);
	void SetDepthStencilView(Texture2DInstance<DSVOption>* depthStencilViewBuffer);

public:
	inline const Texture2DInstance<SRVOption, RTVOption, UAVOption>* GetFilm() const { return m_film; }
	inline const Texture2DInstance<RTVOption>* GetIDFilm() const { return m_idFilm; }
	inline const Texture2DInstance<PureTextureOption>* GetIDStatgingFilm() const { return m_idStagingFilm; }
	inline const Texture2DInstance<DSVOption>* GetDepthStencilViewBuffer() const { return m_depthStencilViewBuffer; }

public:
	void UpdateViewElement();

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

