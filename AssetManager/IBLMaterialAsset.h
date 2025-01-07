#pragma once
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "IGPUAsset.h"

enum class EIBLMaterialTexture
{
	IBL_MATERIAL_TEXTURE_BACKGROUND,
	IBL_MATERIAL_TEXTURE_SPECULAR,
	IBL_MATERIAL_TEXTURE_DIFFUSE,
	IBL_MATERIAL_TEXTURE_BRDF,
};

constexpr size_t IBLMaterialTextureCount = static_cast<size_t>(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF) + 1;

struct SIBLToneMapping
{
	float m_exposure = 1.f;
	float m_gamma = 1.f;
	float m_dummy1 = 0.f;
	float m_dummy2 = 0.f;
};

class DynamicBuffer;

class IBLMaterialAsset : public AAsset, public IGPUAsset
{
public:
	IBLMaterialAsset();
	IBLMaterialAsset(const std::string& assetName);
	~IBLMaterialAsset() override;

protected:
	std::string m_materialTextureName[IBLMaterialTextureCount];
	ScratchTextureAsset* m_materialTexture[IBLMaterialTextureCount];

public:
	const ScratchTextureAsset* const GetScratchTextureAsset(const EIBLMaterialTexture& iblMaterialTexture) const;

public:
	void UpdateIBLBaseTextureAsset(
		EIBLMaterialTexture iblMaterialTextureType,
		IScratchTextureProvider& provider
	);
	void SetIBLMaterialTexture(
		EIBLMaterialTexture iblMaterialTextureType,
		const std::string& iblmaterialTextureIn,
		IScratchTextureProvider& provider
	);
	void SetIBLMaterialTexture(
		EIBLMaterialTexture iblMaterialTextureType,
		ScratchTextureAsset* scratchTextureAsset
	);

protected:
	SIBLToneMapping m_iblToneMappingConstants;
	DynamicBuffer* m_iblToneMappingBuffer;

public:
	const DynamicBuffer* GetIBLToneMappingBuffer() const { return m_iblToneMappingBuffer; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;

public:
	virtual void InitializeGPUAsset(
ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	) override;
};

class IIBLMaterialProvider
{
public:
	virtual IBLMaterialAsset* const GetIBLMaterialAsset(
		const std::string& assetName
	) = 0;
};
