#include "AssetInitializer.h"

#include "AssetManager.h"

#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

#include <d3d11.h>

using namespace std;
using namespace DirectX;

AssetInitializer::AssetInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void AssetInitializer::Visit(StaticMeshAsset* staticMeshAsset)
{
	auto& staticMeshPartsPerLOD = staticMeshAsset->GetStaticMeshPartsPerLOD();
	for (auto& staticMeshPartPerLOD : staticMeshPartsPerLOD)
	{
		staticMeshPartPerLOD.second->Accept(*this);
	}
}

void AssetInitializer::Visit(SkeletalMeshAsset* skeletalMeshAsset)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	skeletalMeshAsset->UpdateBoneAsset(*assetManager);
	auto& skeletalMeshPartsPerLOD = skeletalMeshAsset->GetSkeletalMeshPartsPerLOD();
	for (auto& skeletalMeshPartPerLOD : skeletalMeshPartsPerLOD)
	{
		skeletalMeshPartPerLOD.second->Accept(*this);
	}
}

void AssetInitializer::Visit(StaticMeshPartsData* staticMeshPartsData)
{
	const vector<XMFLOAT3>& positions = staticMeshPartsData->GetPositions();
	const vector<XMFLOAT2>& uvTextures = staticMeshPartsData->GetUVTextures();
	const vector<XMFLOAT3>& normals = staticMeshPartsData->GetNormals();


	staticMeshPartsData->SetVertexBuffer(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(positions.size()), positions.data(), D3D11_BIND_VERTEX_BUFFER));
	staticMeshPartsData->SetUVBuffer(new ConstantBuffer(sizeof(XMFLOAT2), static_cast<UINT>(uvTextures.size()), uvTextures.data(), D3D11_BIND_VERTEX_BUFFER));
	staticMeshPartsData->SetNormalBuffer(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(normals.size()), normals.data(), D3D11_BIND_VERTEX_BUFFER));

	const vector<ConstantBuffer*>& vertexBuffers = staticMeshPartsData->GetVertexConstantBuffers();
	for (ConstantBuffer* vertexBuffer : vertexBuffers)
	{
		vertexBuffer->InitializeBuffer(m_deviceCached);
	}

	const vector<uint32_t>& indices = staticMeshPartsData->GetIndices();
	staticMeshPartsData->SetIndexBuffer(new ConstantBuffer(sizeof(uint32_t), static_cast<UINT>(indices.size()), indices.data(), D3D11_BIND_INDEX_BUFFER));

	ConstantBuffer* const indexBuffer = staticMeshPartsData->GetIndexBuffer();
	indexBuffer->InitializeBuffer(m_deviceCached);
}

void AssetInitializer::Visit(SkeletalMeshPartsData* skeletalMeshPartsData)
{
	const vector<XMFLOAT4>& blendWeights = skeletalMeshPartsData->GetBlendWeights();
	const vector<XMINT4>& blendIndices = skeletalMeshPartsData->GetBlendIndices();

	skeletalMeshPartsData->SetBlendWeightBuffer(new ConstantBuffer(sizeof(XMFLOAT4), static_cast<UINT>(blendWeights.size()), blendWeights.data(), D3D11_BIND_VERTEX_BUFFER));
	skeletalMeshPartsData->SetBlendIndexBuffer(new ConstantBuffer(sizeof(XMINT4), static_cast<UINT>(blendIndices.size()), blendIndices.data(), D3D11_BIND_VERTEX_BUFFER));

	Visit((StaticMeshPartsData*)skeletalMeshPartsData);
}

void AssetInitializer::Visit(BaseTextureAsset* baseTextureAsset)
{
	*baseTextureAsset->GetResourceAddress() = new Texture2DInstance<SRVOption, RTVOption>(
		baseTextureAsset->GetWidth(), baseTextureAsset->GetHeight(), baseTextureAsset->GetArraySize(), 0,
		baseTextureAsset->GetImageBuffers(), baseTextureAsset->GetRowPitchArray(),
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM,
		m_deviceCached, m_deviceContextCached
	);
}

void AssetInitializer::Visit(ScratchTextureAsset* scratchTextureAsset)
{
	*scratchTextureAsset->GetResourceAddress() = new Texture2DInstance<SRVOption, RTVOption>(
		scratchTextureAsset->GetWidth(), scratchTextureAsset->GetHeight(), 
		scratchTextureAsset->GetArraySize(), scratchTextureAsset->GetMipLevels(),
		scratchTextureAsset->GetImageBuffers(), scratchTextureAsset->GetRowPitchArray(),
		NULL, scratchTextureAsset->GetMiscFlag(), D3D11_USAGE_DEFAULT, scratchTextureAsset->GetFormat(),
		m_deviceCached, m_deviceContextCached
	);
}

void AssetInitializer::Visit(ModelMaterialAsset* modelMaterialAsset)
{
	AssetManager* assetManager = AssetManager::GetInstance();
	for (size_t materialTextureType = 0; materialTextureType < ModelMaterialTextureCount; ++materialTextureType)
	{
		modelMaterialAsset->UpdateModelBaseTextureAsset(
			static_cast<EModelMaterialTexture>(materialTextureType),
			*assetManager
		);
	}
	modelMaterialAsset->UpdateModelMaterialTexturesFromNames();

	DynamicBuffer* const modelTextureSettingBuffer = modelMaterialAsset->GetModelTextureSettingBuffer();
	modelTextureSettingBuffer->InitializeBuffer(m_deviceCached);
}

void AssetInitializer::Visit(IBLMaterialAsset* iblMaterialAsset)
{
	AssetManager* assetManager = AssetManager::GetInstance();
	for (size_t materialTextureType = 0; materialTextureType < IBLMaterialTextureCount; ++materialTextureType)
	{
		iblMaterialAsset->UpdateIBLBaseTextureAsset(
			static_cast<EIBLMaterialTexture>(materialTextureType),
			*assetManager
		);
	}
	DynamicBuffer* const iblToneMappingBuffer = iblMaterialAsset->GetIBLToneMappingBuffer();
	iblToneMappingBuffer->InitializeBuffer(m_deviceCached);
}


void AssetInitializer::Visit(BoneAsset* boneAsset)
{
}

void AssetInitializer::Visit(AnimationAsset* animationAsset)
{
}
