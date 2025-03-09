#pragma once
#include "AComponent.h"
#include "ACollidableFrustum.h"
#include "ViewEntity.h"

#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

struct SCameraEntity
{
	SCameraEntity(
		const uint32_t& width,
		const uint32_t& height,
		const float& focalDistance,
		const float& aperture,
		const float& nearZ,
		const float& farZ
	);
	uint32_t m_width;
	uint32_t m_height;
	float m_focalDistance;
	float m_aperture;
	float m_nearZ;
	float m_farZ;
	float dummy[2];
};

class CameraComponent : public AComponent, public ACollidableFrustum
{
public:
	CameraComponent(
		const std::string& componentName,
		const uint32_t& componentID,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& scale,
		const uint32_t& width = GDefaultViewWidth,
		const uint32_t& height = GDefaultViewHeight,
		const float& fovAngle = GDefaultFovAngle,
		const float& nearZ = GDefaultNearZ,
		const float& farZ = GDefaultFarZ
	);
	~CameraComponent() override = default;

protected:
	SViewEntity m_viewEntity;
	SCameraEntity m_cameraEntity;
	DynamicBuffer m_viewEntityBuffer;
	DynamicBuffer m_cameraEntityBuffer;
	AtomicFlag m_isViewEntityUpdated = false;
	AtomicFlag m_isCameraEntityUpdated = false;

protected:
	bool m_isUseDOF = false;

public:
	const bool& GetIsUseDOF() { return m_isUseDOF; }
	void SetUseDOF(const bool& isUseDOF) { m_isUseDOF = isUseDOF; }

public:
	const SCameraEntity& GetCameraEntity() { return m_cameraEntity; };
	DynamicBuffer& GetViewEntityBuffer() { return m_viewEntityBuffer; }
	inline AtomicFlag& GetViewUpdatedFlag() { return m_isViewEntityUpdated; }
	DynamicBuffer& GetCameraEntityBuffer() { return m_cameraEntityBuffer; }
	inline AtomicFlag& GetCameraUpdatedFlag() { return m_isCameraEntityUpdated; }
	void SetCameraEntity(const float& focalDistance, const float& aperture, const float& nearZ, const float& farZ);

protected:
	Texture2DInstance<SRVOption, RTVOption, UAVOption>	m_film;
	Texture2DInstance<SRVOption, RTVOption>				m_filteredFilm;
	Texture2DInstance<RTVOption>						m_idFilm;
	Texture2DInstance<PureTextureOption>				m_idStagingFilm;
	Texture2DInstance<SRVOption, DSVOption>				m_depthStencilView;

protected:
	float m_fovAngle;
	D3D11_VIEWPORT m_viewport;

public:
	inline const float& GetFovAngle() const { return m_fovAngle; }
	inline const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }
	void SetFovAngle(const float& fovAngle);
	void SetViewport(const uint32_t& width, const uint32_t& height);

public:
	inline Texture2DInstance<SRVOption, RTVOption, UAVOption>& GetFilm() { return m_film; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetFilteredFilm() { return m_filteredFilm; }
	inline Texture2DInstance<RTVOption>& GetIDFilm() { return m_idFilm; }
	inline Texture2DInstance<PureTextureOption>& GetIDStatgingFilm() { return m_idStagingFilm; }
	inline Texture2DInstance<SRVOption, DSVOption>& GetDepthStencilView() { return m_depthStencilView; }

protected:
	DirectX::XMMATRIX m_viewMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX m_projMatrix = DirectX::XMMatrixIdentity();

public:
	const DirectX::XMMATRIX& GetViewMatrix() const { return m_viewMatrix; }
	const DirectX::XMMATRIX& GetProjMatrix() const { return m_projMatrix; }

public:
	virtual void UpdateEntity(ID3D11DeviceContext* deviceContext);	
	void UpdateViewEntity(ID3D11DeviceContext* deviceContext);

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void OnCollide(ICollisionAcceptor*) override;

protected:
	virtual void UpdateBoundingProperty() override;
};

