#include "EnvironmentActor.h"

#include "AssetManager.h"

#include "BaseSphereMeshAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "MapAsset.h"
#include "ACamera.h"

#include "GlobalVariable.h"
#include "UploadableBufferManager.h"
using namespace std;

string EnvironmentActor::EnvironmentActorKind = "Environment Actor";

EnvironmentActor::EnvironmentActor()
	: AActor(EnvironmentActor::EnvironmentActorKind)
{
	static size_t EnvironmentActorCount = 0;

	HDRToneMappingConstantBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<UploadBuffer<SHDRToneMappingConstant>>();
	SetToneMappingConstant(1.f, 1.f);

	EnvironmentActorCount++;
	ObjectName = EnvironmentActor::EnvironmentActorKind + to_string(EnvironmentActorCount);

	EnvironmentActorPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Environment_Solid);
}

EnvironmentActor::~EnvironmentActor()
{

}

void EnvironmentActor::SetToneMappingConstant(const float& ExposureIn, const float& GammaIn)
{
	SHDRToneMappingConstant ToneMappingConstant;
	ToneMappingConstant.Exposure = ExposureIn;
	ToneMappingConstant.Gamma = GammaIn;
	HDRToneMappingConstantBuffer->SetStagedData(ToneMappingConstant);
}

void EnvironmentActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void EnvironmentActor::Render(MapAsset* MapAssetIn)
{
	AActor::Render(MapAssetIn);

	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	if (CurrentCamera != nullptr && EnvironmentMeshAsset != nullptr)
	{
		ID3D11RenderTargetView* RTVs[]{ CurrentCamera->GetSDRSceneRTV() };
		const std::vector<ID3D11Buffer*> VertexBuffers = EnvironmentMeshAsset->GetVertexBuffers();
		const std::vector<UINT> Strides = EnvironmentMeshAsset->GetStrides();
		const std::vector<UINT> Offsets = EnvironmentMeshAsset->GetOffsets();
		ID3D11Buffer* IndexBuffer = EnvironmentMeshAsset->GetIndexBuffer();
		const DXGI_FORMAT IndexFormat = EnvironmentMeshAsset->GetIndexFormat();
		const UINT IndexCount = EnvironmentMeshAsset->GetIndexCount();
		ID3D11Buffer* VSConstBuffers[] = { CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer()};
		ID3D11Buffer* PSConstBuffers[] = { HDRToneMappingConstantBuffer->GetBuffer(), };
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

void EnvironmentActor::OnSerialize(FILE* FileIn)
{
	AObject::OnSerialize(FileIn);

	AssetNameSerializeHelper(EnvironmentMeshAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBackgroundEXRTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentSpecularDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentDiffuseDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBRDFDDSTextureAsset, FileIn);

	SHDRToneMappingConstant StagedData = HDRToneMappingConstantBuffer->GetStagedData();
	
	fwrite(&StagedData.Exposure, sizeof(float), 1, FileIn);
	fwrite(&StagedData.Gamma, sizeof(float), 1, FileIn);
}

void EnvironmentActor::OnDeserialize(FILE* FileIn)
{
	AObject::OnDeserialize(FileIn);

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

	EnvironmentMeshAsset = App::GAssetManager->GetManagingBaseMesh(EnvironmentMeshAssetName);
	EnvironmentBackgroundEXRTextureAsset = App::GAssetManager->GetManagingEXRTexture(EnvironmentBackgroundEXRTextureAssetName);
	EnvironmentSpecularDDSTextureAsset = App::GAssetManager->GetManagingDDSTexture(EnvironmentSpecularDDSTextureAssetName);
	EnvironmentDiffuseDDSTextureAsset = App::GAssetManager->GetManagingDDSTexture(EnvironmentDiffuseDDSTextureAssetName);
	EnvironmentBRDFDDSTextureAsset = App::GAssetManager->GetManagingDDSTexture(EnvironmentBRDFDDSTextureAssetName);

	float Exposure, Gamma;
	fread(&Exposure, sizeof(float), 1, FileIn);
	fread(&Gamma, sizeof(float), 1, FileIn);

	SetToneMappingConstant(Exposure, Gamma);
}

template<typename T>
void EnvironmentActor::AssetNameSerializeHelper(T AssetIn, FILE* FileIn)
{
	const string AssetName = AssetIn != nullptr ? AssetIn->GetAssetName() : "";
	AAssetFile::SerializeString(AssetName, FileIn);


}
