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

AssetInitializer::AssetInitializer(AssetManager* assetManager, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_assetManager(assetManager), m_deviceCached(device), m_deviceContextCached(deviceContext)
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
	skeletalMeshAsset->UpdateBoneAsset(*m_assetManager);
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
	const vector<XMFLOAT3>& tangents = staticMeshPartsData->GetTangents();


	staticMeshPartsData->AddVertexBuffer(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(positions.size()), positions.data(), D3D11_BIND_VERTEX_BUFFER));
	staticMeshPartsData->AddVertexBuffer(new ConstantBuffer(sizeof(XMFLOAT2), static_cast<UINT>(uvTextures.size()), uvTextures.data(), D3D11_BIND_VERTEX_BUFFER));
	staticMeshPartsData->AddVertexBuffer(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(normals.size()), normals.data(), D3D11_BIND_VERTEX_BUFFER));
	staticMeshPartsData->AddVertexBuffer(new ConstantBuffer(sizeof(XMFLOAT3), static_cast<UINT>(tangents.size()), tangents.data(), D3D11_BIND_VERTEX_BUFFER));

	const vector<ConstantBuffer*>& vertexBuffers = staticMeshPartsData->GetVertexBuffers();
	for (ConstantBuffer* vertexBuffer : vertexBuffers)
	{
		const D3D11_SUBRESOURCE_DATA vertexSubresourceData = vertexBuffer->GetSubResourceData();
		vertexBuffer->InitializeBuffer(m_deviceCached, &vertexSubresourceData);
	}

	const vector<uint32_t>& indices = staticMeshPartsData->GetIndices();
	staticMeshPartsData->SetIndexBuffer(new ConstantBuffer(sizeof(uint32_t), static_cast<UINT>(indices.size()), indices.data(), D3D11_BIND_INDEX_BUFFER));

	ConstantBuffer* const indexBuffer = staticMeshPartsData->GetIndexBuffer();
	const D3D11_SUBRESOURCE_DATA indexSubresourceData = indexBuffer->GetSubResourceData();
	indexBuffer->InitializeBuffer(m_deviceCached, &indexSubresourceData);
}

void AssetInitializer::Visit(SkeletalMeshPartsData* skeletalMeshPartsData)
{
	Visit((StaticMeshPartsData*)skeletalMeshPartsData);

	const vector<XMFLOAT4>& blendWeights = skeletalMeshPartsData->GetBlendWeights();
	const vector<XMINT4>& blendIndices = skeletalMeshPartsData->GetBlendIndices();

	ConstantBuffer* const blendWeightBuffer = new ConstantBuffer(sizeof(XMFLOAT4), static_cast<UINT>(blendWeights.size()), blendWeights.data(), D3D11_BIND_VERTEX_BUFFER);
	ConstantBuffer* const blendIndexBuffer = new ConstantBuffer(sizeof(XMINT4), static_cast<UINT>(blendIndices.size()), blendIndices.data(), D3D11_BIND_VERTEX_BUFFER);

	const D3D11_SUBRESOURCE_DATA blendWeightSubresourceData = blendWeightBuffer->GetSubResourceData();
	const D3D11_SUBRESOURCE_DATA blendIndexSubresourceData = blendIndexBuffer->GetSubResourceData();

	blendWeightBuffer->InitializeBuffer(m_deviceCached, &blendWeightSubresourceData);
	blendIndexBuffer->InitializeBuffer(m_deviceCached, &blendIndexSubresourceData);

	skeletalMeshPartsData->AddVertexBuffer(blendWeightBuffer);
	skeletalMeshPartsData->AddVertexBuffer(blendIndexBuffer);


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
	for (size_t materialTextureType = 0; materialTextureType < ModelMaterialTextureCount; ++materialTextureType)
	{
		modelMaterialAsset->UpdateModelBaseTextureAsset(
			static_cast<EModelMaterialTexture>(materialTextureType),
			*m_assetManager
		);
	}
	modelMaterialAsset->UpdateModelMaterialTexturesFromNames();

	DynamicBuffer* const modelTextureSettingBuffer = modelMaterialAsset->GetModelTextureSettingBuffer();
	const D3D11_SUBRESOURCE_DATA modelTextureSettingSubresourceData = modelTextureSettingBuffer->GetSubResourceData();
	modelTextureSettingBuffer->InitializeBuffer(m_deviceCached, &modelTextureSettingSubresourceData);
}

void AssetInitializer::Visit(IBLMaterialAsset* iblMaterialAsset)
{
	for (size_t materialTextureType = 0; materialTextureType < IBLMaterialTextureCount; ++materialTextureType)
	{
		iblMaterialAsset->UpdateIBLBaseTextureAsset(
			static_cast<EIBLMaterialTexture>(materialTextureType),
			*m_assetManager
		);
	}
	DynamicBuffer* const iblToneMappingBuffer = iblMaterialAsset->GetIBLToneMappingBuffer();
	const D3D11_SUBRESOURCE_DATA iblToneMpaaingSubsourceData = iblToneMappingBuffer->GetSubResourceData();
	iblToneMappingBuffer->InitializeBuffer(m_deviceCached, &iblToneMpaaingSubsourceData);
}


void AssetInitializer::Visit(BoneAsset* boneAsset)
{
}

void AssetInitializer::Visit(AnimationAsset* animationAsset)
{
}
