#include "EnvironementActorDrawer.h"
#include "EnvironmentActor.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "UIVariable.h"

#include "AssetSelectHelper.h"

using namespace std;
using namespace ImGui;

EnvironementActorDrawer::EnvironementActorDrawer(EnvironmentActor* EnvironmentActorIn)
    : AInformationDrawer(EnvironmentActorIn)
{}

void EnvironementActorDrawer::DrawInformation()
{
    DrawBackgroundEXRTexture();
    DrawIBLTextures();
    DrawToneMappingConstant();
}

void EnvironementActorDrawer::DrawBackgroundEXRTexture()
{
    SeparatorText("Background Texture");
    const std::unordered_map<std::string, std::shared_ptr<EXRTextureAsset>>& ManagingEXRTextures = App::GAssetManager->GetManagingEXRTextures();
    EXRTextureAsset* EXRTextureAssetInstance = ObjectCached->GetEnvironmentBackgroundEXRTextureAsset();
    shared_ptr<EXRTextureAsset> SelectedEXRTexture = DrawAndSelectNormalTexture(
        ManagingEXRTextures, EXRTextureAssetInstance,
        "Background Texture Asset(EXR)", "Select Background Texture Asset"
    );
    if (SelectedEXRTexture != nullptr) ObjectCached->SetEnvironmentBackgroundEXRTextureAsset(SelectedEXRTexture);
}

void EnvironementActorDrawer::DrawIBLTextures()
{
    SeparatorText("IBL Texture");
    const std::unordered_map<std::string, std::shared_ptr<DDSTextureAsset>>& ManagingDDSTextures = App::GAssetManager->GetManagingDDSTextures();

    DDSTextureAsset* DDSSpecularTextureAssetInstance = ObjectCached->GetEnvironmentSpecularDDSTextureAsset();
    DDSTextureAsset* DDSDiffuseTextureAssetInstance = ObjectCached->GetEnvironmentDiffuseDDSTextureAsset();
    DDSTextureAsset* DDSBRDFTextureAssetInstance = ObjectCached->GetEnvironmentBRDFDDSTextureAsset();

    shared_ptr<DDSTextureAsset> SelectedSpecularTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSSpecularTextureAssetInstance,
        "Specular Texture Asset(DDS)", "Select Specular Texture Asset"
    );
    shared_ptr<DDSTextureAsset> SelectedDiffuseTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSDiffuseTextureAssetInstance,
        "Diffuse Texture Asset(DDS)", "Select Diffuse Texture Asset"
    );
    shared_ptr<DDSTextureAsset> SelectedBRDFTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSBRDFTextureAssetInstance,
        "BRDF Texture Asset(DDS)", "Select BRDF Texture Asset"
    );

    if (SelectedSpecularTexture != nullptr) ObjectCached->SetEnvironmentSpecularDDSTextureAsset(SelectedSpecularTexture);
    if (SelectedDiffuseTexture != nullptr) ObjectCached->SetEnvironmentDiffuseDDSTextureAsset(SelectedDiffuseTexture);
    if (SelectedBRDFTexture != nullptr) ObjectCached->SetEnvironmentBRDFDDSTextureAsset(SelectedBRDFTexture);
}

void EnvironementActorDrawer::DrawToneMappingConstant()
{
    const SHDRToneMappingConstant& StagedToneMappingConstant = ObjectCached->GetHDRToneMappingConstantBuffer()->GetStagedData();

    float Exposure = StagedToneMappingConstant.Exposure;
    float Gamma = StagedToneMappingConstant.Gamma;

    SeparatorText("Tone Mapping");

    bool UpdateFlag = false;
    UpdateFlag |= DragFloat("Exposure", &Exposure, 0.01f, 0.f, 5.f);
    UpdateFlag |= DragFloat("Gamma", &Gamma, 0.01f, 0.f, 5.f);

    if (UpdateFlag)
    {
        ObjectCached->SetToneMappingConstant(Exposure, Gamma);
    }
}


template<typename T>
inline shared_ptr<T> EnvironementActorDrawer::DrawAndSelectNormalTexture(
    const unordered_map<string, shared_ptr<T>>& ManagingTexturesIn,
    T* CurrentSelected, const char* StrId, const char* PreviewText
)
{
    if (CurrentSelected != nullptr)
    {
        ID3D11ShaderResourceView* SRV = CurrentSelected->GetSRV();
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        SRV->GetDesc(&SRVDesc);
        if (SRVDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
        {
            ImGui::Image(SRV, UISize::FileSize);
        }
        else
        {
            ImGui::Image(nullptr, UISize::FileSize);
        }
    }
    else
    {
        ImGui::Image(nullptr, UISize::FileSize);
    }

    SameLine();

    shared_ptr<T> Result = AssetSelectHelper::SelectAsset(ManagingTexturesIn, CurrentSelected, StrId, StrId, PreviewText);

    return Result;
}
