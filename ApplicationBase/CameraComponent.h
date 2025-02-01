#pragma once
#include "AViewComponent.h"
#include "CollidableFrustum.h"

class CameraComponent : public AViewComponent, public CollidableFrustum
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
	Texture2DInstance<SRVOption, RTVOption, UAVOption>* m_film = nullptr;
	Texture2DInstance<RTVOption>*						m_idFilm = nullptr;
	Texture2DInstance<PureTextureOption>*				m_idStagingFilm = nullptr;
	Texture2DInstance<DSVOption>*						m_depthStencilViewBuffer = nullptr;

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
	virtual void UpdateViewEntity() override;

public:
	virtual void Accept(IComponentVisitor* visitor) override;
	virtual void OnCollide(ICollisionAcceptor*) override;
};

