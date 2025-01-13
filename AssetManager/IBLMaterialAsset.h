#pragma once
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"

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

class IBLMaterialAsset : public AAsset
{
public:
	IBLMaterialAsset();
	IBLMaterialAsset(const std::string& assetName);
	~IBLMaterialAsset() override;

protected:
	std::string m_materialTextureName[IBLMaterialTextureCount];
	ScratchTextureAsset* m_materialTexture[IBLMaterialTextureCount]{ nullptr, nullptr, nullptr, nullptr };

public:
	const ScratchTextureAsset* const GetScratchTextureAsset(const EIBLMaterialTexture& iblMaterialTexture) const;

public:
	void UpdateIBLBaseTextureAsset(
		EIBLMaterialTexture iblMaterialTextureType,
		IScratchTextureProvider& provider
	);

	void SetIBLMaterialTextureName(
		EIBLMaterialTexture iblMaterialTextureType,
		const std::string& iblmaterialTextureIn
	);
	void SetIBLMaterialTexture(
		EIBLMaterialTexture iblMaterialTextureType,
		ScratchTextureAsset* scratchTextureAsset
	);

protected:
	SIBLToneMapping m_iblToneMappingConstants;
	DynamicBuffer* m_iblToneMappingBuffer;

public:
	void UpdateIBLToneMappingConstant(
		ID3D11DeviceContext* deviceContext,
		const float& exposure,
		const float& gamma
	);

	inline DynamicBuffer* GetIBLToneMappingBuffer() const { return m_iblToneMappingBuffer; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;
};

class IIBLMaterialProvider
{
public:
	virtual IBLMaterialAsset* const GetIBLMaterialAsset(
		const std::string& assetName
	) = 0;
};
