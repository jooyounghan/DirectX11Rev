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
	const XMFLOAT3 MeshObjectPosition = GetAbsolutePosition();
	const XMFLOAT3 DeltaPosition = XMFLOAT3(
		From.x - MeshObjectPosition.x,
		From.y - MeshObjectPosition.y,
		From.z - MeshObjectPosition.z
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

	if (CurrentCamera != nullptr && MeshAssetInstance != nullptr)
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
		const vector<UINT> MaterialIndexes = MeshAssetInstance->GetMaterialIndex(LODLevel);

		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);


#pragma region MeshObjectPSOCached
		{
			DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
				VertexBuffers.data(),
				Strides.data(),
				Offsets.data()
			);

			vector<ID3D11RenderTargetView*> RTVs = { CurrentCamera->GetSDRSceneRTV() };
			ID3D11DepthStencilView* DSV = CurrentCamera->GetSceneDSV();

			MeshObjectPSOCached->SetPipelineStateObject(RTVs.size(), RTVs.data(), Viewport, DSV);

			for (size_t PartIdx = 0; PartIdx < MaterialIndexes.size(); ++PartIdx)
			{
				vector<ID3D11Buffer*> MeshObjectVSConstants = GetMeshObjectVSConstants(PartIdx);
				vector<ID3D11Buffer*> MeshObjectHSConstants = GetMeshObjectHSConstants(PartIdx);
				vector<ID3D11Buffer*> MeshObjectDSConstants = GetMeshObjectDSConstants(PartIdx);
				vector<ID3D11Buffer*> MeshObjectPSConstants = GetMeshObjectPSConstants(PartIdx);
				vector<ID3D11ShaderResourceView*> MeshObjectVSSRVs = GetMeshObjectVSSRVs(PartIdx);
				vector<ID3D11ShaderResourceView*> MeshObjectHSSRVs = GetMeshObjectHSSRVs(PartIdx);
				vector<ID3D11ShaderResourceView*> MeshObjectDSSRVs = GetMeshObjectDSSRVs(PartIdx);
				vector<ID3D11ShaderResourceView*> MeshObjectPSSRVs = GetMeshObjectPSSRVs(PartIdx);

				MeshObjectPSOCached->SetVSConstantBuffers(0, MeshObjectVSConstants.size(), MeshObjectVSConstants.data());
				MeshObjectPSOCached->SetHSConstantBuffers(0, MeshObjectHSConstants.size(), MeshObjectHSConstants.data());
				MeshObjectPSOCached->SetDSConstantBuffers(0, MeshObjectDSConstants.size(), MeshObjectDSConstants.data());
				MeshObjectPSOCached->SetPSConstantBuffers(0, MeshObjectPSConstants.size(), MeshObjectPSConstants.data());
				MeshObjectPSOCached->SetVSShaderResourceViews(0, MeshObjectVSSRVs.size(), MeshObjectVSSRVs.data());
				MeshObjectPSOCached->SetHSShaderResourceViews(0, MeshObjectHSSRVs.size(), MeshObjectHSSRVs.data());
				MeshObjectPSOCached->SetDSShaderResourceViews(0, MeshObjectDSSRVs.size(), MeshObjectDSSRVs.data());
				MeshObjectPSOCached->SetPSShaderResourceViews(0, MeshObjectPSSRVs.size(), MeshObjectPSSRVs.data());

				MeshObjectPSOCached->CheckPipelineValidation();

				DeviceContextCached->DrawIndexed(IndexCounts[PartIdx], IndexOffsets[PartIdx], 0);
				Performance::GTotalIndexCount += IndexTotalCount;

				MeshObjectPSOCached->ResetVSConstantBuffers(0, MeshObjectVSConstants.size());
				MeshObjectPSOCached->ResetHSConstantBuffers(0, MeshObjectHSConstants.size());
				MeshObjectPSOCached->ResetDSConstantBuffers(0, MeshObjectDSConstants.size());
				MeshObjectPSOCached->ResetPSConstantBuffers(0, MeshObjectPSConstants.size());
				MeshObjectPSOCached->ResetVSShaderResourceViews(0, MeshObjectVSSRVs.size());
				MeshObjectPSOCached->ResetHSShaderResourceViews(0, MeshObjectHSSRVs.size());
				MeshObjectPSOCached->ResetDSShaderResourceViews(0, MeshObjectDSSRVs.size());
				MeshObjectPSOCached->ResetPSShaderResourceViews(0, MeshObjectPSSRVs.size());
			}
		}
#pragma endregion

#pragma region PickingIDSolidStaticPSOCached
		{
			DeviceContextCached->IASetVertexBuffers(0, 1, VertexBuffers.data(), Strides.data(), Offsets.data());

			vector<ID3D11RenderTargetView*> RTVs = { CurrentCamera->GetIdSelectRTV() };
			ID3D11DepthStencilView* DSV = CurrentCamera->GetIdSelectDSV();

			vector<ID3D11Buffer*> VSConstBuffers = { CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer() };
			vector<ID3D11Buffer*> PSConstBuffers = { PickingIDBufferCached };

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

void AMeshObject::Update(const float& DeltaTimeIn)
{
	AAttachableObject::Update(DeltaTimeIn);
}

void AMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	size_t MaterialCount = MaterialAssetInstances.size();
	fwrite(&MaterialCount, sizeof(size_t), 1, FileIn);

	for (auto& MaterialAssetInstance : MaterialAssetInstances)
	{
		if (MaterialAssetInstance != nullptr)
		{
			AAssetFile::SerializeString(MaterialAssetInstance->GetAssetName(), FileIn);
		}
		else
		{
			AAssetFile::SerializeString("", FileIn);
		}
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
