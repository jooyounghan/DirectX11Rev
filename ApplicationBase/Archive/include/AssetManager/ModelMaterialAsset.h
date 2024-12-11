#pragma once
#include "BaseTextureAsset.h"
#include "DirectXMath/DirectXMath.h"

enum class EModelMaterialTexture
{
	MODEL_MATERIAL_TEXTURE_AMBIENTOCCULUSION,
	MODEL_MATERIAL_TEXTURE_SPECULAR,
	MODEL_MATERIAL_TEXTURE_DIFFUSE,
	MODEL_MATERIAL_TEXTURE_ROUGHNESS,
	MODEL_MATERIAL_TEXTURE_METALIC,
	MODEL_MATERIAL_TEXTURE_NORMAL,
	MODEL_MATERIAL_TEXTURE_HEIGHT,
	MODEL_MATERIAL_TEXTURE_EMISSIVE
};

constexpr size_t ModelMaterialTextureCount = static_cast<size_t>(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_EMISSIVE) + 1;

class ModelMaterialAsset : public AAsset
{
public:
	ModelMaterialAsset() = default;
	ModelMaterialAsset(const std::string& assetName);
	virtual ~ModelMaterialAsset();

protected:
	std::string m_materialTextureName[ModelMaterialTextureCount];
	std::shared_ptr<BaseTextureAsset> m_materialTexture[ModelMaterialTextureCount];

protected:
	 DirectX::XMFLOAT3 m_f0 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	float m_heightScale = 1.f;

public:
	MakeGetter(m_f0, FresnelConstant);
	MakeGetter(m_heightScale, HeightScale);

public:
	void UpdateModelBaseTextureAsset(
		EModelMaterialTexture modelMaterialTextureType,
		IBaseTextureProvider& provider
	);
	void SetModelMaterialTexture(
		EModelMaterialTexture modelMaterialTextureType, 
		const std::string& modelMaterialTextureIn, 
		IBaseTextureProvider& provider
	);

	void SetModelMaterialTexture(
		EModelMaterialTexture modelMaterialTextureType,
		std::shared_ptr<BaseTextureAsset> baseTextureAsset
	);

public:
	void SetModelMaterialProperties(const DirectX::XMFLOAT3& f0, const float& heightScale);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IModelMaterialProvider
{
public:
	virtual std::shared_ptr<ModelMaterialAsset> GetModelMaterialAsset(
		const std::string& assetName
	) = 0;
};

