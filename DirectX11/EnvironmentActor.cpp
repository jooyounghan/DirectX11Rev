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
	: AActor(MapAssetInstance)
{
	static size_t EnvironmentActorCount = 0;

	AutoZeroMemory(HDRToneMappingConstant);
	HDRToneMappingConstant.Exposure = 1.f;
	HDRToneMappingConstant.Gamma = 1.f;

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

	fwrite(&HDRToneMappingConstant.Exposure, sizeof(float), 1, FileIn);
	fwrite(&HDRToneMappingConstant.Gamma, sizeof(float), 1, FileIn);
}

void EnvironmentActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string EnvironmentMeshAssetName;
	string EnvironmentBackgroundEXRTextureAssetName;
	string EnvironmentSpecularDDSTextureAssetName;
	string EnvironmentDiffuseDDSTextureAssetName;
	string EnvironmentBRDFDDSTextureAssetName;

	AAssetFile::DeserializeString(EnvironmentMeshAssetName, FileIn);
	AAssetFile::DeserializeString(EnvironmentBackgroundEXRTextureAssetName, FileIn);
	AAssetFile::DeserializeString(EnvironmentSpecularDDSTextureAssetName, FileIn);
	AAssetFile::DeserializeString(EnvironmentDiffuseDDSTextureAssetName, FileIn);
	AAssetFile::DeserializeString(EnvironmentBRDFDDSTextureAssetName, FileIn);

	EnvironmentMeshAsset = AssetManagerIn->GetManagingBaseMesh(EnvironmentMeshAssetName);
	EnvironmentBackgroundEXRTextureAsset = AssetManagerIn->GetManagingEXRTexture(EnvironmentBackgroundEXRTextureAssetName);
	EnvironmentSpecularDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentSpecularDDSTextureAssetName);
	EnvironmentDiffuseDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentDiffuseDDSTextureAssetName);
	EnvironmentBRDFDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentBRDFDDSTextureAssetName);


	fread(&HDRToneMappingConstant.Exposure, sizeof(float), 1, FileIn);
	fread(&HDRToneMappingConstant.Gamma, sizeof(float), 1, FileIn);
}

template<typename T>
void EnvironmentActor::AssetNameSerializeHelper(T AssetIn, FILE* FileIn)
{
	const string AssetName = AssetIn != nullptr ? AssetIn->GetAssetName() : "";
	AAssetFile::SerializeString(AssetName, FileIn);


}
