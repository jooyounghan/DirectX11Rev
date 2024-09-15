#include "SkeletalMeshObjectInformationDrawer.h"
#include "SkeletalMeshObject.h"

#include "GlobalVariable.h"

#include "AssetManager.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace ImGui;


using namespace std;
using namespace ImGui;

SkeletalMeshObjectInformationDrawer::SkeletalMeshObjectInformationDrawer(SkeletalMeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{
}

void SkeletalMeshObjectInformationDrawer::DrawInformation()
{
    SeparatorText("Skeletal Mesh Object");

    const std::unordered_map<std::string, std::shared_ptr<SkeletalMeshAsset>>& ManagingSkeletalMeshes = AssetManagerCached->GetManagingSkeletalMeshes();

    const AMeshAsset* MeshAssetInstance = ObjectCached->GetSkeletalMeshAssetInstance();

    ImGui::ColorButton("TEST", UIColor::GBlack, NULL, UISize::FileSize);

    SameLine();

    if (ImGui::BeginCombo("Skeletal Mesh Asset", MeshAssetInstance != nullptr ? MeshAssetInstance->GetAssetName().c_str() : "Choose Skeletal Mesh Asset"))
    {
        for (auto& SkeletalMesh : ManagingSkeletalMeshes)
        {
            if (ImGui::Selectable(SkeletalMesh.first.c_str()))
            {
                ObjectCached->SetSkeletalMeshAssetInstance(SkeletalMesh.second);
            }
        }

        ImGui::EndCombo();
    }
}
