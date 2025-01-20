#include "IBLMaterialAssetCreateFlowNode.h"

#include "AssetManager.h"
#include "AssetWrapper.h"
#include "IBLMaterialAsset.h"

using namespace std;

IBLMaterialAssetCreateFlowNode::IBLMaterialAssetCreateFlowNode(
	const ImVec2& leftTop, const float& radius, 
	const ImVec2& referencedOrigin, 
	ID3D11Device* const* deviceAddress,
	ID3D11DeviceContext* const* deviceContextAddress,
	AssetManager* assetManager
)
	: FlowNode(
		"Create IBL Material Asset", leftTop, radius, referencedOrigin, 
		{"Asset Path", "File Name", "Background", "Specular Cube Map", "Diffuse Cube Map", "BRDR", "Exposure", "Gamma"}),
	m_deviceAddressCached(deviceAddress), m_deviceContextAddressCached(deviceContextAddress), m_assetManagerCached(assetManager)
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			return GetMinNodeSize();
		});
}

void IBLMaterialAssetCreateFlowNode::ExecuteImpl()
{
	auto inputVariables = GetInputVariables();

	const string& assetPath = get<0>(inputVariables);
	const string& assetName = get<1>(inputVariables);
	ScratchTextureAsset* background = get<2>(inputVariables);
	ScratchTextureAsset* specular = get<3>(inputVariables);
	ScratchTextureAsset* diffuse = get<4>(inputVariables);
	ScratchTextureAsset* brdf = get<5>(inputVariables);
	const float& exposure = get<6>(inputVariables);
	const float& gamma = get<7>(inputVariables);

	IBLMaterialAsset* iblMaterialAsset = new IBLMaterialAsset(assetName);
	iblMaterialAsset->SetIBLMaterialTexture(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BACKGROUND, background);
	iblMaterialAsset->SetIBLMaterialTexture(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_SPECULAR, specular);
	iblMaterialAsset->SetIBLMaterialTexture(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_DIFFUSE, diffuse);
	iblMaterialAsset->SetIBLMaterialTexture(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_BRDF, brdf);
	iblMaterialAsset->SetIBLToneMappingConstant(exposure, gamma);

	m_assetManagerCached->AddAssetHelper(*m_deviceAddressCached, *m_deviceContextAddressCached,
		EAssetType::ASSET_TYPE_IBL_MATERIAL, assetPath + "/" + assetName, iblMaterialAsset
	);
	AAssetWriter::SaveAssets(assetPath, EAssetType::ASSET_TYPE_IBL_MATERIAL, { iblMaterialAsset });
}
