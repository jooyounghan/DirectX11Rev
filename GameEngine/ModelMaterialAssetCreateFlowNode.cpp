#include "ModelMaterialAssetCreateFlowNode.h"

#include "AssetManager.h"
#include "AssetWrapper.h"
#include "ModelMaterialAsset.h"

using namespace std;
using namespace DirectX;

ModelMaterialAssetCreateFlowNode::ModelMaterialAssetCreateFlowNode(
	const ImVec2& leftTop, const float& radius,
	const ImVec2& referencedOrigin,
	ID3D11Device* const* deviceAddress,
	ID3D11DeviceContext* const* deviceContextAddress
)
	: FlowNode("Create Model Material Asset", leftTop, radius, referencedOrigin,
		{ "Asset Path", "File Name", "Ambient Occulusion", "Specular", "Diffuse", "Roughness", "Metalic", "Normal", "Height", "Emissive", "Fresnel", "Height Scale"}),
	m_deviceAddressCached(deviceAddress), m_deviceContextAddressCached(deviceContextAddress)
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			return GetMinNodeSize();
		});
}

void ModelMaterialAssetCreateFlowNode::ExecuteImpl()
{
	AssetManager* assetManager = AssetManager::GetInstance();

	auto inputVariables = GetInputVariables();

	const string& assetPath = get<0>(inputVariables);
	const string& assetName = get<1>(inputVariables);
	BaseTextureAsset* ambientOcculusion = get<2>(inputVariables);
	BaseTextureAsset* specular = get<3>(inputVariables);
	BaseTextureAsset* diffuse = get<4>(inputVariables);
	BaseTextureAsset* roughness = get<5>(inputVariables);
	BaseTextureAsset* metalic = get<6>(inputVariables);
	BaseTextureAsset* normal = get<7>(inputVariables);
	BaseTextureAsset* height = get<8>(inputVariables);
	BaseTextureAsset* emissive = get<9>(inputVariables);
	XMFLOAT3 fresnel = get<10>(inputVariables);
	float heightScale = get<11>(inputVariables);

	ModelMaterialAsset* modelMaterialAsset = new ModelMaterialAsset(assetName);

	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_AMBIENTOCCULUSION, ambientOcculusion);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_SPECULAR, specular);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_DIFFUSE, diffuse);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_ROUGHNESS, roughness);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_METALIC, metalic);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_NORMAL, normal);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_HEIGHT, height);
	modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_EMISSIVE, emissive);
	modelMaterialAsset->SetModelMaterialConstants(fresnel, heightScale);

	assetManager->AddAssetHelper(*m_deviceAddressCached, *m_deviceContextAddressCached,
		EAssetType::ASSET_TYPE_MODEL_MATERIAL, assetPath + "/" + assetName, modelMaterialAsset
	);

	AAssetWriter::SaveAssets(assetPath, EAssetType::ASSET_TYPE_MODEL_MATERIAL, { modelMaterialAsset });
}
