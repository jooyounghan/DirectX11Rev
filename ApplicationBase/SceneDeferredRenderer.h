#pragma once
#include "ASceneRenderer.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include "DSVOption.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class SceneDeferredRenderer : public ASceneRenderer
{
public:
	SceneDeferredRenderer(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager,
		CameraComponent* const* cameraComponentAddress,
		Scene* const* sceneAddress
	);
	~SceneDeferredRenderer() override = default;


protected:
	Texture2DInstance<SRVOption, RTVOption> m_positionGBuffer;
	Texture2DInstance<SRVOption, RTVOption> m_specularGBuffer;
	Texture2DInstance<SRVOption, RTVOption> m_diffuseGBuffer;
	Texture2DInstance<SRVOption, RTVOption> m_aoMetallicRoughnessGBuffer;
	Texture2DInstance<SRVOption, RTVOption> m_normalGBuffer;
	Texture2DInstance<SRVOption, RTVOption> m_emissiveGBuffer;

protected:
	std::vector<ID3D11ShaderResourceView*> m_gBufferShaderResourceViews;
	std::vector<ID3D11RenderTargetView*> m_gBufferRenderTargetViews;

public:
	inline Texture2DInstance<SRVOption, RTVOption>& GetPositionGBuffer() { return m_positionGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetSpecularGBuffer() { return m_specularGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetDiffuseGBuffer() { return m_diffuseGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetAoMetallicRoughnessGBuffer() { return m_aoMetallicRoughnessGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetNormalGBuffer() { return m_normalGBuffer; }
	inline Texture2DInstance<SRVOption, RTVOption>& GetEmissiveGBuffer() { return m_emissiveGBuffer; }

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
		CameraComponent* cameraComponent
	) const override;
	virtual void RenderMeshPartHandler(const size_t& idx) override;
};

