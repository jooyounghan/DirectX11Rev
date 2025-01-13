#pragma once
#include "Asset.h"
#include "BaseTextureAsset.h"

#include <DirectXMath.h>

class DynamicBuffer;

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

struct SModelTextureSetting
{
	BOOL m_isTextureSet[ModelMaterialTextureCount];
	DirectX::XMFLOAT3 m_fresnelConstant;
	float m_heightScale;
};

class ModelMaterialAsset : public AAsset
{
public:
	ModelMaterialAsset();
	ModelMaterialAsset(const std::string& assetName);
	~ModelMaterialAsset() override;

protected:
	std::string m_materialTextureName[ModelMaterialTextureCount];
	const BaseTextureAsset* m_materialTexture[ModelMaterialTextureCount] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

protected:
	SModelTextureSetting m_modelTextureSetting;
	DynamicBuffer* m_modelTextureSettingBuffer;

public:
	void SetModelMaterialConstants(const DirectX::XMFLOAT3& f0, const float& heightScale);
	void UpdateModelMaterialTexturesFromNames();
	inline DynamicBuffer* GetModelTextureSettingBuffer() const { return m_modelTextureSettingBuffer; }
	inline const BaseTextureAsset* GetModelMaterialTexture(EModelMaterialTexture modelMaterialTextureType) const{ return m_materialTexture[static_cast<size_t>(modelMaterialTextureType)]; }
	
public:
	void UpdateModelBaseTextureAsset(
		EModelMaterialTexture modelMaterialTextureType,
		IBaseTextureProvider& provider
	);

	void SetModelMaterialTextureName(
		EModelMaterialTexture modelMaterialTextureType, 
		const std::string& modelMaterialTextureIn		
	);
	void SetModelMaterialTexture(
		EModelMaterialTexture modelMaterialTextureType,
		const BaseTextureAsset* baseTextureASsetIn
	);


public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	virtual void Accept(IAssetVisitor* visitor) override;;
};

class IModelMaterialProvider
{
public:
	virtual ModelMaterialAsset* const GetModelMaterialAsset(
		const std::string& assetName
	) = 0;
};

