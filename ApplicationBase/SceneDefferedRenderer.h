#pragma once
#include "ASceneRenderer.h"

struct ID3D11Device;

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class RTVOption;

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class SceneDefferedRenderer : public ASceneRenderer
{
public:
	SceneDefferedRenderer(
		ID3D11Device* const* deviceAddress,
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~SceneDefferedRenderer() override;


protected:
	Texture2DInstance<SRVOption, RTVOption>* m_positionGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_baseColorGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_aoMetallicRoughnessGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_normalGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_emissiveGBuffer = nullptr;

protected:
	std::vector<ID3D11ShaderResourceView*> m_gBufferShaderResourceViews;
	std::vector<ID3D11RenderTargetView*> m_gBufferRenderTargetViews;

public:
	inline Texture2DInstance<SRVOption, RTVOption>* GetPositionGBuffer() const { return m_positionGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetBaseColorGBuffer() const { return m_baseColorGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetAoMetallicRoughnessGBuffer() const { return m_aoMetallicRoughnessGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetNormalGBuffer() const { return m_normalGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetEmissiveGBuffer() const { return m_emissiveGBuffer; }

public:
	inline const std::vector<ID3D11ShaderResourceView*> GetGBufferSRVs() const { return m_gBufferShaderResourceViews; }
	inline const std::vector<ID3D11RenderTargetView*> GetGBufferRTVs() const { return m_gBufferRenderTargetViews; }

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

protected:
	void ApplyGBuffer(ID3D11DeviceContext* const deviceContext, const CameraComponent* const cameraComponent);

public:
	virtual void PostProcess() override;
};

