#include "EnvironementActorDrawer.h"
#include "EnvironmentActor.h"

#include "AssetManager.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

using namespace std;
using namespace ImGui;

EnvironementActorDrawer::EnvironementActorDrawer(EnvironmentActor* EnvironmentActorIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(EnvironmentActorIn), AssetManagerCached(AssetManagerIn)
{}

void EnvironementActorDrawer::DrawInformation()
{
    SeparatorText("Environment Actor");

    const std::unordered_map<std::string, std::shared_ptr<EXRTextureAsset>>& ManagingEXRTextures = AssetManagerCached->GetManagingEXRTextures();
    EXRTextureAsset* EXRTextureAssetInstance = ObjectCached->GetEnvironmentBackgroundEXRTextureAsset();

    if (EXRTextureAssetInstance != nullptr)
    {
        ImGui::Image(EXRTextureAssetInstance->GetSRV(), UISize::FileSize);
    }
    else
    {
        ColorButton("EXRTextureAseetThumbnail", UIColor::GBlack, NULL, UISize::FileSize);
    }
    SameLine();

    if (BeginCombo("Background Texture Asset(EXR)", EXRTextureAssetInstance != nullptr ? EXRTextureAssetInstance->GetAssetName().c_str() : "Choose Background Texture Asset"))
    {
        for (auto& ManagingEXRTexture : ManagingEXRTextures)
        {
            if (Selectable(ManagingEXRTexture.first.c_str()))
            {
                ObjectCached->SetEnvironmentBackgroundEXRTextureAsset(ManagingEXRTexture.second);
            }
        }

        EndCombo();
    }
}
