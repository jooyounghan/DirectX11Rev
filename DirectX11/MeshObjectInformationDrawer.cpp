#include "MeshObjectInformationDrawer.h"
#include "MeshObject.h"

#include "GlobalVariable.h"

#include "AssetManager.h"
#include "AMeshAsset.h"

#include "SkeletalMeshAsset.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace ImGui;

MeshObjectInformationDrawer::MeshObjectInformationDrawer(MeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{

}

void MeshObjectInformationDrawer::DrawInformation()
{
	SeparatorText("Mesh Object");

    const std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>>& ManagingStaticMeshes = AssetManagerCached->GetManagingStaticMeshes();
    const std::unordered_map<std::string, std::shared_ptr<SkeletalMeshAsset>>& ManagingSkeletalMeshes = AssetManagerCached->GetManagingSkeletalMeshes();

    const AMeshAsset* MeshAssetInstance = ObjectCached->GetMeshAssetInstance();

    ImGui::ColorButton("TEST", UIColor::GBlack, NULL, UISize::FileSize);
    //Image(nullptr, UISize::FileSize);
    SameLine();

    if (ImGui::BeginCombo("Mesh Asset", MeshAssetInstance != nullptr ? MeshAssetInstance->GetAssetName().c_str() : "Choose Mesh Asset"))
    {
        for (auto& StaticMesh : ManagingStaticMeshes)
        {
            if (ImGui::Selectable(StaticMesh.first.c_str()))
            {
                ObjectCached->SetMeshAssetInstance(StaticMesh.second);
            }
        }
        for (auto& SkeletalMesh : ManagingSkeletalMeshes)
        {
            if (ImGui::Selectable(SkeletalMesh.first.c_str()))
            {
                ObjectCached->SetMeshAssetInstance(SkeletalMesh.second);
            }
        }

        ImGui::EndCombo();
    }
}
