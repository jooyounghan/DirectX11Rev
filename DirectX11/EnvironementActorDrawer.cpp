#include "EnvironementActorDrawer.h"
#include "EnvironmentActor.h"

#include "AssetManager.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "UIVariable.h"

#include "AssetSelectHelper.h"

using namespace std;
using namespace ImGui;

EnvironementActorDrawer::EnvironementActorDrawer(EnvironmentActor* EnvironmentActorIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(EnvironmentActorIn), AssetManagerCached(AssetManagerIn)
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
    const std::unordered_map<std::string, std::shared_ptr<EXRTextureAsset>>& ManagingEXRTextures = AssetManagerCached->GetManagingEXRTextures();
    EXRTextureAsset* EXRTextureAssetInstance = ObjectCached->GetEnvironmentBackgroundEXRTextureAsset();
    shared_ptr<EXRTextureAsset> SelectedEXRTexture = DrawAndSelectNormalTexture(
        ManagingEXRTextures, EXRTextureAssetInstance,
        "Background Texture Asset(EXR)", "Choose Background Texture Asset"
    );
    if (SelectedEXRTexture != nullptr) ObjectCached->SetEnvironmentBackgroundEXRTextureAsset(SelectedEXRTexture);
}

void EnvironementActorDrawer::DrawIBLTextures()
{
    SeparatorText("IBL Texture");
    const std::unordered_map<std::string, std::shared_ptr<DDSTextureAsset>>& ManagingDDSTextures = AssetManagerCached->GetManagingDDSTextures();

    DDSTextureAsset* DDSSpecularTextureAssetInstance = ObjectCached->GetEnvironmentSpecularDDSTextureAsset();
    DDSTextureAsset* DDSDiffuseTextureAssetInstance = ObjectCached->GetEnvironmentDiffuseDDSTextureAsset();
    DDSTextureAsset* DDSBRDFTextureAssetInstance = ObjectCached->GetEnvironmentBRDFDDSTextureAsset();

    shared_ptr<DDSTextureAsset> SelectedSpecularTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSSpecularTextureAssetInstance,
        "Specular Texture Asset(DDS)", "Choose Specular Texture Asset"
    );
    shared_ptr<DDSTextureAsset> SelectedDiffuseTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSDiffuseTextureAssetInstance,
        "Diffuse Texture Asset(DDS)", "Choose Diffuse Texture Asset"
    );
    shared_ptr<DDSTextureAsset> SelectedBRDFTexture = DrawAndSelectNormalTexture(
        ManagingDDSTextures, DDSBRDFTextureAssetInstance,
        "BRDF Texture Asset(DDS)", "Choose BRDF Texture Asset"
    );

    if (SelectedSpecularTexture != nullptr) ObjectCached->SetEnvironmentSpecularDDSTextureAsset(SelectedSpecularTexture);
    if (SelectedDiffuseTexture != nullptr) ObjectCached->SetEnvironmentDiffuseDDSTextureAsset(SelectedDiffuseTexture);
    if (SelectedBRDFTexture != nullptr) ObjectCached->SetEnvironmentBRDFDDSTextureAsset(SelectedBRDFTexture);
}

void EnvironementActorDrawer::DrawToneMappingConstant()
{
    SeparatorText("Tone Mapping");
    SHDRToneMappingConstant* HDRToneMappingConstant = ObjectCached->GetPointerHDRToneMappingConstant();
    DragFloat("Exposure", &HDRToneMappingConstant->Exposure, 0.01f, 0.f, 5.f);
    DragFloat("Gamma", &HDRToneMappingConstant->Gamma, 0.01f, 0.f, 5.f);
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
