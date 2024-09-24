#include "EnvironmentActor.h"

#include "AssetManager.h"

#include "BaseSphereMeshAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "Camera.h"

using namespace std;

const char* EnvironmentActor::EnvironmentActorIdentifier = "Environment Actor";

EnvironmentActor::EnvironmentActor(MapAsset* MapAssetInstance)
	: AActor(MapAssetInstance), Exposure(HDRToneMappingConstant.Exposure), Gamma(HDRToneMappingConstant.Gamma)
{
	static size_t EnvironmentActorCount = 0;

	AutoZeroMemory(HDRToneMappingConstant);

	EnvironmentActorCount++;
	ObjectName = EnvironmentActorIdentifier + to_string(EnvironmentActorCount);
	PlaceableKind = EPlaceableObjectKind::ENVIORNMENT_ACTOR_KIND;

	EnvironmentActorPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Environment_Solid);
}

EnvironmentActor::~EnvironmentActor()
{

}

void EnvironmentActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void EnvironmentActor::Render()
{
	AActor::Render();

	Camera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	if (CurrentCamera != nullptr && EnvironmentMeshAsset != nullptr)
	{
		ID3D11RenderTargetView* RTVs[]{ CurrentCamera->GetSDRSceneRTV() };
		const std::vector<ID3D11Buffer*> VertexBuffers = EnvironmentMeshAsset->GetVertexBuffers();
		const std::vector<UINT> Strides = EnvironmentMeshAsset->GetStrides();
		const std::vector<UINT> Offsets = EnvironmentMeshAsset->GetOffsets();
		ID3D11Buffer* IndexBuffer = EnvironmentMeshAsset->GetIndexBuffer();
		const DXGI_FORMAT IndexFormat = EnvironmentMeshAsset->GetIndexFormat();
		const UINT IndexCount = EnvironmentMeshAsset->GetIndexCount();
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->ViewProjBuffer.GetBuffer(), TransformationBuffer.GetBuffer() };
		ID3D11Buffer* PSConstBuffers[] = { HDRToneMappingConstantBuffer.GetBuffer(), };
		ID3D11ShaderResourceView* PSSRVs[] = { EnvironmentBackgroundEXRTextureAsset != nullptr ? EnvironmentBackgroundEXRTextureAsset->GetSRV() : nullptr };

		const D3D11_VIEWPORT* Viewport = &CurrentCamera->GetViewport();
		ID3D11DepthStencilView* DSV = CurrentCamera->GetSceneDSV();

#pragma region EnvironmentActorPSOCached
		EnvironmentActorPSOCached->SetPipelineStateObject(1, RTVs, Viewport, DSV);

		DeviceContextCached->IASetVertexBuffers(0, static_cast<UINT>(VertexBuffers.size()),
			VertexBuffers.data(),
			Strides.data(),
			Offsets.data()
		);
		DeviceContextCached->IASetIndexBuffer(IndexBuffer, IndexFormat, 0);

		EnvironmentActorPSOCached->SetVSConstantBuffers(0, 2, VSConstBuffers);
		EnvironmentActorPSOCached->SetPSConstantBuffers(0, 1, PSConstBuffers);
		EnvironmentActorPSOCached->SetPSShaderResourceViews(0, 1, PSSRVs);

#ifdef _DEBUG
		EnvironmentActorPSOCached->CheckPipelineValidation();
#endif // DEBUG

		DeviceContextCached->DrawIndexed(static_cast<UINT>(IndexCount), 0, 0);
		Performance::GTotalIndexCount += IndexCount;

		EnvironmentActorPSOCached->ResetVSConstantBuffers(0, 2);
		EnvironmentActorPSOCached->ResetPSConstantBuffers(0, 1);
		EnvironmentActorPSOCached->ResetPSShaderResourceViews(0, 1);
#pragma endregion
	}
}

void EnvironmentActor::Update(const float& DeltaTimeIn)
{
	RelativeScale.x = 1000.f;
	RelativeScale.y = 1000.f;
	RelativeScale.z = 1000.f;
	AActor::Update(DeltaTimeIn);
	HDRToneMappingConstantBuffer.Upload(HDRToneMappingConstant);
}

void EnvironmentActor::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	AssetNameSerializeHelper(EnvironmentMeshAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBackgroundEXRTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentSpecularDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentDiffuseDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBRDFDDSTextureAsset, FileIn);
}

void EnvironmentActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string EnvironmentMeshAssetName;
	string EnvironmentBackgroundEXRTextureAssetName;
	string EnvironmentSpecularDDSTextureAssetName;
	string EnvironmentDiffuseDDSTextureAssetName;
	string EnvironmentBRDFDDSTextureAssetName;

	DeserializeString(EnvironmentMeshAssetName, FileIn);
	DeserializeString(EnvironmentBackgroundEXRTextureAssetName, FileIn);
	DeserializeString(EnvironmentSpecularDDSTextureAssetName, FileIn);
	DeserializeString(EnvironmentDiffuseDDSTextureAssetName, FileIn);
	DeserializeString(EnvironmentBRDFDDSTextureAssetName, FileIn);

	EnvironmentMeshAsset = AssetManagerIn->GetManagingBaseMesh(EnvironmentMeshAssetName);
	EnvironmentBackgroundEXRTextureAsset = AssetManagerIn->GetManagingEXRTexture(EnvironmentMeshAssetName);
	EnvironmentSpecularDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
	EnvironmentDiffuseDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
	EnvironmentBRDFDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
}

template<typename T>
void EnvironmentActor::AssetNameSerializeHelper(T AssetIn, FILE* FileIn)
{
	const string AssetName = AssetIn != nullptr ? AssetIn->GetAssetName() : "";
	SerializeString(AssetName, FileIn);
}
