#include "AMeshObject.h"

#include "AssetManager.h"
#include "AMeshAsset.h"
#include "BaseTextureAsset.h"
#include "DDSTextureAsset.h"
#include "MaterialAsset.h"

#include "MapAsset.h"
#include "ACamera.h"

#include "EnvironmentActor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

AMeshObject::AMeshObject(
	MapAsset* MapAssetInstance,
	const string& AttachableKindIn
)
	: AAttachableObject(MapAssetInstance, AttachableKindIn)
{
}

AMeshObject::~AMeshObject()
{
}

void AMeshObject::SetMaterialAsset(const size_t& MaterialIdx, std::shared_ptr<MaterialAsset> MaterialAssetIn)
{
	if (MaterialAssetInstances.size() > MaterialIdx)
	{
		MaterialAssetInstances[MaterialIdx] = MaterialAssetIn;
	}
}

MaterialAsset* AMeshObject::GetMaterialAssetInstance(const size_t& MaterialIdx)
{
	MaterialAsset* Result = nullptr;
	if (MaterialAssetInstances.size() > MaterialIdx)
	{
		Result = MaterialAssetInstances[MaterialIdx].get();
	}
	return Result;
}

size_t AMeshObject::GetLODLevel(
	const XMFLOAT3& From, 
	const float& MaxDistance, 
	const size_t& MaxLODCount, 
	const size_t SteepLevel
)
{
	const XMFLOAT3 SkeletalMeshObjectPosition = GetAbsolutePosition();
	const XMFLOAT3 DeltaPosition = XMFLOAT3(
		From.x - SkeletalMeshObjectPosition.x,
		From.y - SkeletalMeshObjectPosition.y,
		From.z - SkeletalMeshObjectPosition.z
	);

	const float Distance = sqrt(MathematicalHelper::InnerProduct(DeltaPosition, DeltaPosition));
	const float x = min(MaxDistance, Distance) / MaxDistance;

	const float LODFloat = pow(x, (1.f / SteepLevel)) * MaxLODCount;
	return static_cast<size_t>(LODFloat);
}

void AMeshObject::Render()
{
	AAttachableObject::Render();

	AMeshAsset* MeshAssetInstance = GetMeshAssetInstance();

	ACamera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	EnvironmentActor* CurrentEnvironment = MapAssetCached->GetEnvironmentActorInstance();

	if (CurrentCamera != nullptr && MeshAssetInstance != nullptr && CurrentEnvironment != nullptr)
	{
		const size_t LODLevel = GetLODLevel(
			CurrentCamera->GetAbsolutePosition(),
			*CurrentCamera->GetPointerFarZ(),
			MeshAssetInstance->GetLODCount(),
			3
		);

		const D3D11_VIEWPORT* Viewport = &CurrentCamera->GetViewport();

		const std::vector<ID3D11Buffer*> VertexBuffers = MeshAssetInstance->GetVertexBuffers(LODLevel);
		const std::vector<UINT> Strides = MeshAssetInstance->GetStrides();
		const std::vector<UINT> Offsets = MeshAssetInstance->GetOffsets();

		ID3D11Buffer* IndexBuffer = MeshAssetInstance->GetIndexBuffer(LODLevel);
		const DXGI_FORMAT IndexFormat = MeshAssetInstance->GetIndexFormat();
		const UINT IndexTotalCount = MeshAssetInstance->GetIndexCount(LODLevel);

		const vector<UINT> IndexCounts = MeshAssetInstance->GetIndexCountsForPart(LODLevel);
		const vector<UINT> IndexOffsets = MeshAssetInstance->GetIndexOffsetsForPart(LODLevel);
		const vector<UINT> MaterialIndex = MeshAssetInstance->GetMaterialIndex(LODLevel);

		vector<ID3D11Buffer*> VSConstBuffers;
		vector<ID3D11Buffer*> PSConstBuffers;

		ID3D11Buffer* ViewProjGPUBuffer = CurrentCamera->GetViewProjBuffer()->GetBuffer();
		VSConstBuffers.push_back(ViewProjGPUBuffer);
		VSConstBuffers.push_back(TransformationBuffer->GetBuffer());

		PSConstBuffers.push_back(ViewProjGPUBuffer);

		vector<ID3D11ShaderResourceView*> EnvironmentSRVs;

		DDSTextureAsset* SpecularDDS = CurrentEnvironment->GetEnvironmentSpecularDDSTextureAsset();
		DDSTextureAsset* DiffuseDDS = CurrentEnvironment->GetEnvironmentDiffuseDDSTextureAsset();
		DDSTextureAsset* BRDFDDS = CurrentEnvironment->GetEnvironmentBRDFDDSTextureAsset();

		EnvironmentSRVs.push_back(SpecularDDS != nullptr ? SpecularDDS->GetSRV() : nullptr);
		EnvironmentSRVs.push_back(DiffuseDDS != nullptr ? DiffuseDDS->GetSRV() : nullptr);
		EnvironmentSRVs.push_back(BRDFDDS != nullptr ? BRDFDDS->GetSRV() : nullptr);

		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);
		DeviceContextCached->IASetVertexBuffers(0, VertexBuffers.size(),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);


#pragma region MeshObjectPSOCached
		{
			vector<ID3D11RenderTargetView*> RTVs;
			RTVs.push_back(CurrentCamera->GetSDRSceneRTV());

			ID3D11DepthStencilView* DSV = CurrentCamera->GetSceneDSV();

			MeshObjectPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, DSV);

			MeshObjectPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
			MeshObjectPSOCached->SetPSConstantBuffers(0, PSConstBuffers.size(), PSConstBuffers.data());
			MeshObjectPSOCached->SetPSShaderResourceViews(0, EnvironmentSRVs.size(), EnvironmentSRVs.data());

			for (size_t PartIdx = 0; PartIdx < MaterialIndex.size(); ++PartIdx)
			{
				vector<ID3D11Buffer*> VSConstBuffersPerPart;
				vector<ID3D11Buffer*> PSConstBuffersPerPart;

				vector<ID3D11ShaderResourceView*> VSMaterialSRVs;
				vector<ID3D11ShaderResourceView*> PSMaterialSRVs;

				const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIndex[PartIdx]];
				if (MaterialInstance != nullptr)
				{
					VSConstBuffersPerPart.push_back(MaterialInstance->GetMaterialDataBuffer()->GetBuffer());
					PSConstBuffersPerPart.push_back(MaterialInstance->GetMaterialDataBuffer()->GetBuffer());

					const shared_ptr<BaseTextureAsset> AO = MaterialInstance->GetAmbientOcculusionTextureAsset();
					const shared_ptr<BaseTextureAsset> Specualr = MaterialInstance->GetSpecularTextureAsset();
					const shared_ptr<BaseTextureAsset> Diffuse = MaterialInstance->GetDiffuseTextureAsset();
					const shared_ptr<BaseTextureAsset> Roughness = MaterialInstance->GetRoughnessTextureAsset();
					const shared_ptr<BaseTextureAsset> Metalic = MaterialInstance->GetMetalicTextureAsset();
					const shared_ptr<BaseTextureAsset> Normal = MaterialInstance->GetNormalTextureAsset();
					const shared_ptr<BaseTextureAsset> Height = MaterialInstance->GetHeightTextureAsset();
					const shared_ptr<BaseTextureAsset> Emissive = MaterialInstance->GetEmissiveTextureAsset();

					VSMaterialSRVs.push_back(Normal != nullptr ? Normal->GetSRV() : nullptr);
					VSMaterialSRVs.push_back(Height != nullptr ? Height->GetSRV() : nullptr);

					PSMaterialSRVs.push_back(AO != nullptr ? AO->GetSRV() : nullptr);
					PSMaterialSRVs.push_back(Specualr != nullptr ? Specualr->GetSRV() : nullptr);
					PSMaterialSRVs.push_back(Diffuse != nullptr ? Diffuse->GetSRV() : nullptr);
					PSMaterialSRVs.push_back(Roughness != nullptr ? Roughness->GetSRV() : nullptr);
					PSMaterialSRVs.push_back(Metalic != nullptr ? Metalic->GetSRV() : nullptr);
					PSMaterialSRVs.push_back(Emissive != nullptr ? Emissive->GetSRV() : nullptr);
				}
				else
				{
					VSMaterialSRVs.insert(VSMaterialSRVs.end(), { nullptr, nullptr });
					PSMaterialSRVs.insert(PSMaterialSRVs.end(), { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr });
				}

				MeshObjectPSOCached->SetVSConstantBuffers(VSConstBuffers.size(), VSConstBuffersPerPart.size(), VSConstBuffersPerPart.data());
				MeshObjectPSOCached->SetPSConstantBuffers(PSConstBuffers.size(), PSConstBuffersPerPart.size(), PSConstBuffersPerPart.data());

				MeshObjectPSOCached->SetVSShaderResourceViews(0, VSMaterialSRVs.size(), VSMaterialSRVs.data());
				MeshObjectPSOCached->SetPSShaderResourceViews(EnvironmentSRVs.size(), PSMaterialSRVs.size(), PSMaterialSRVs.data());

				MeshObjectPSOCached->CheckPipelineValidation();

				DeviceContextCached->DrawIndexed(IndexCounts[PartIdx], IndexOffsets[PartIdx], 0);
				Performance::GTotalIndexCount += IndexTotalCount;

				MeshObjectPSOCached->ResetVSConstantBuffers(VSConstBuffers.size(), VSConstBuffersPerPart.size());
				MeshObjectPSOCached->ResetPSConstantBuffers(PSConstBuffers.size(), PSConstBuffersPerPart.size());

				MeshObjectPSOCached->ResetVSShaderResourceViews(0, VSMaterialSRVs.size());
				MeshObjectPSOCached->ResetPSShaderResourceViews(EnvironmentSRVs.size(), PSMaterialSRVs.size());
			}

			MeshObjectPSOCached->ResetVSConstantBuffers(0, VSConstBuffers.size());
			MeshObjectPSOCached->ResetPSConstantBuffers(0, PSConstBuffers.size());
			MeshObjectPSOCached->ResetPSShaderResourceViews(0, EnvironmentSRVs.size());
		}
#pragma endregion


#pragma region PickingIDSolidStaticPSOCached
		{
			vector<ID3D11RenderTargetView*> RTVs;
			RTVs.push_back(CurrentCamera->GetIdSelectRTV());

			ID3D11DepthStencilView* DSV = CurrentCamera->GetIdSelectDSV();

			vector<ID3D11Buffer*> PSConstBuffers;
			PSConstBuffers.push_back(PickingIDBufferCached);

			PickingIDSolidPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, DSV);

			PickingIDSolidPSOCached->SetVSConstantBuffers(0, VSConstBuffers.size(), VSConstBuffers.data());
			PickingIDSolidPSOCached->SetPSConstantBuffers(0, PSConstBuffers.size(), PSConstBuffers.data());

			PickingIDSolidPSOCached->CheckPipelineValidation();

			DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexTotalCount), 0, 0);

			PickingIDSolidPSOCached->ResetVSConstantBuffers(0, VSConstBuffers.size());
			PickingIDSolidPSOCached->ResetPSConstantBuffers(0, PSConstBuffers.size());
		}
#pragma endregion
	}
}

void AMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	size_t MaterialCount = MaterialAssetInstances.size();
	fwrite(&MaterialCount, sizeof(size_t), 1, FileIn);

	for (auto& MaterialAssetInstance : MaterialAssetInstances)
	{
		AAssetFile::SerializeString(MaterialAssetInstance->GetAssetName(), FileIn);
	}
}

void AMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	size_t MaterialCount;
	fread(&MaterialCount, sizeof(size_t), 1, FileIn);

	MaterialAssetInstances.resize(MaterialCount);
	for (size_t idx = 0; idx < MaterialCount; ++idx)
	{
		string MaterialAssetName;
		AAssetFile::DeserializeString(MaterialAssetName, FileIn);
		
		MaterialAssetInstances[idx] = AssetManagerIn->GetManagingMaterial(MaterialAssetName);
	}
}
