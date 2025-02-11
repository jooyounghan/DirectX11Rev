#pragma once
#include "ASceneRenderer.h"

struct ID3D11Device;

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class RTVOption;

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class SceneDeferredRenderer : public ASceneRenderer
{
public:
	SceneDeferredRenderer(
		ID3D11Device* const* deviceAddress,
		ID3D11DeviceContext* const* deviceContextAddress,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~SceneDeferredRenderer() override;


protected:
	Texture2DInstance<SRVOption, RTVOption>* m_positionGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_specularGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_diffuseGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_aoMetallicRoughnessGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_normalGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_emissiveGBuffer = nullptr;
	Texture2DInstance<SRVOption, RTVOption>* m_fresnelReflectanceGBuffer = nullptr;

protected:
	std::vector<ID3D11ShaderResourceView*> m_gBufferShaderResourceViews;
	std::vector<ID3D11RenderTargetView*> m_gBufferRenderTargetViews;

public:
	inline Texture2DInstance<SRVOption, RTVOption>* GetPositionGBuffer() const { return m_positionGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetSpecularGBuffer() const { return m_specularGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetDiffuseGBuffer() const { return m_diffuseGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetAoMetallicRoughnessGBuffer() const { return m_aoMetallicRoughnessGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetNormalGBuffer() const { return m_normalGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetEmissiveGBuffer() const { return m_emissiveGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>* GetFresnelReflectanceGBuffer() const { return m_fresnelReflectanceGBuffer; }

public:
	inline const std::vector<ID3D11ShaderResourceView*> GetGBufferSRVs() const { return m_gBufferShaderResourceViews; }
	inline const std::vector<ID3D11RenderTargetView*> GetGBufferRTVs() const { return m_gBufferRenderTargetViews; }

public:
	virtual void Visit(StaticMeshComponent* staticMeshAsset) override;
	virtual void Visit(SkeletalMeshComponent* skeletalMeshAsset) override;

public:
	virtual void Visit(CameraComponent* cameraComponent) override;

public:
	virtual void ClearRenderTargets() override;
	virtual void PostProcess() override;

protected:
	virtual void ApplyRenderTargetsWithID(
		ID3D11DeviceContext* const deviceContext, 
		const CameraComponent* const cameraComponent
	) const override;
	virtual void RenderMeshPartHandler(const size_t& idx) override;
};

