
#include "ModelMaterialAsset.h"

using namespace std;
using namespace DirectX;

ModelMaterialAsset::ModelMaterialAsset()
	: m_modelTextureSettingBuffer(new DynamicBuffer(sizeof(SModelTextureSetting), 1))
{
	AutoZeroMemory(m_modelTextureSetting);
}

ModelMaterialAsset::ModelMaterialAsset(const string& assetName)
	: AAsset(assetName), m_modelTextureSettingBuffer(new DynamicBuffer(sizeof(SModelTextureSetting), 1))
{
	AutoZeroMemory(m_modelTextureSetting);
}

ModelMaterialAsset::~ModelMaterialAsset()
{
	delete m_modelTextureSettingBuffer;
}

void ModelMaterialAsset::UpdateModelBaseTextureAsset(EModelMaterialTexture modelMaterialTextureType, IBaseTextureProvider& provider)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetBaseTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void ModelMaterialAsset::SetModelMaterialTextureName(EModelMaterialTexture modelMaterialTextureType, const std::string& modelMaterialTextureIn)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = modelMaterialTextureIn;
}

void ModelMaterialAsset::SetModelMaterialTexture(EModelMaterialTexture modelMaterialTextureType, const BaseTextureAsset* baseTextureAssetIn)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = baseTextureAssetIn->GetAssetName();
	m_materialTexture[materialTextureIdx] = baseTextureAssetIn;
}

void ModelMaterialAsset::UpdateModelTextureSetting(DirectX::XMFLOAT3* f0, const float* heightScale, ID3D11DeviceContext* const deviceContext)
{
	m_modelTextureSetting.m_fresnelConstant = (f0 != nullptr) ? *f0 : m_modelTextureSetting.m_fresnelConstant;
	m_modelTextureSetting.m_heightScale = (heightScale != nullptr) ? *heightScale : m_modelTextureSetting.m_heightScale;

	for (size_t idx = 0; idx < ModelMaterialTextureCount; ++idx)
	{
		m_modelTextureSetting.m_isTextureSet[idx] = (m_materialTexture[idx] != nullptr);
	}
	m_modelTextureSettingBuffer->Upload(deviceContext, sizeof(SModelTextureSetting), 1, &m_modelTextureSetting);
}


void ModelMaterialAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < ModelMaterialTextureCount; ++materialIdx)
	{
		SerializeAssetName(m_materialTexture[materialIdx], fileIn);
	}
	SerializeHelper::SerializeElement(m_modelTextureSetting, fileIn);
}

void ModelMaterialAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < ModelMaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	m_modelTextureSetting = DeserializeHelper::DeserializeElement<SModelTextureSetting>(fileIn);
}

void ModelMaterialAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}

void ModelMaterialAsset::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_modelTextureSettingBuffer->Initialize(device);
	UpdateModelTextureSetting(nullptr, nullptr, deviceContext);
}

