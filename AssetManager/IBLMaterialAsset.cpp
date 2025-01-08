#include "pch.h"
#include "IBLMaterialAsset.h"
#include "DynamicBuffer.h"

using namespace std;
using namespace DirectX;

IBLMaterialAsset::IBLMaterialAsset()
 : AAsset(), m_iblToneMappingBuffer(new DynamicBuffer(sizeof(SIBLToneMapping), 1))
{
}

IBLMaterialAsset::IBLMaterialAsset(const string& assetName)
	: AAsset(assetName), m_iblToneMappingBuffer(new DynamicBuffer(sizeof(SIBLToneMapping), 1))
{
}

IBLMaterialAsset::~IBLMaterialAsset()
{
	delete m_iblToneMappingBuffer;
}

const ScratchTextureAsset* const IBLMaterialAsset::GetScratchTextureAsset(const EIBLMaterialTexture& iblMaterialTexture) const
{
	return m_materialTexture[static_cast<size_t>(iblMaterialTexture)];
}

void IBLMaterialAsset::UpdateIBLBaseTextureAsset(
	EIBLMaterialTexture iblMaterialTextureType, 
	IScratchTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(iblMaterialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetScratchTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void IBLMaterialAsset::SetIBLMaterialTexture(
	EIBLMaterialTexture iblMaterialTextureType, 
	const string& iblmaterialTextureIn, 
	IScratchTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(iblMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = iblmaterialTextureIn;
	UpdateIBLBaseTextureAsset(iblMaterialTextureType, provider);
	m_isModified = true;
}

void IBLMaterialAsset::SetIBLMaterialTexture(EIBLMaterialTexture iblMaterialTextureType, ScratchTextureAsset* scratchTextureAsset)
{
	const size_t& materialTextureIdx = static_cast<size_t>(iblMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = scratchTextureAsset->GetAssetName();
	m_materialTexture[materialTextureIdx] = scratchTextureAsset;
}


void IBLMaterialAsset::UpdateIBLToneMappingConstant(ID3D11DeviceContext* deviceContext, const float& exposure, const float& gamma)
{
	m_iblToneMappingConstants.m_exposure = exposure;
	m_iblToneMappingConstants.m_gamma = gamma;
	m_iblToneMappingBuffer->Upload(deviceContext, sizeof(SIBLToneMapping), 1, &m_iblToneMappingConstants);
}

void IBLMaterialAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < IBLMaterialTextureCount; ++materialIdx)
	{
		SerializeAssetName(m_materialTexture[materialIdx], fileIn);
	}
	fwrite(&m_iblToneMappingConstants.m_exposure, sizeof(float), 1, fileIn);
	fwrite(&m_iblToneMappingConstants.m_gamma, sizeof(float), 1, fileIn);
}

void IBLMaterialAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < IBLMaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	fread(&m_iblToneMappingConstants.m_exposure, sizeof(float), 1, fileIn);
	fread(&m_iblToneMappingConstants.m_gamma, sizeof(float), 1, fileIn);

}

void IBLMaterialAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}

void IBLMaterialAsset::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_iblToneMappingBuffer->Initialize(device);
	m_iblToneMappingBuffer->Upload(deviceContext, sizeof(SIBLToneMapping), 1, &m_iblToneMappingConstants);
}

