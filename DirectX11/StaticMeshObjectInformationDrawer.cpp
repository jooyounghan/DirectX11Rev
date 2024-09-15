#include "StaticMeshObjectInformationDrawer.h"
#include "StaticMeshObject.h"

#include "GlobalVariable.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace ImGui;

StaticMeshObjectInformationDrawer::StaticMeshObjectInformationDrawer(StaticMeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{
}

void StaticMeshObjectInformationDrawer::DrawInformation()
{
	SeparatorText("Static Mesh Object");

    const std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>>& ManagingStaticMeshes = AssetManagerCached->GetManagingStaticMeshes();

    const StaticMeshAsset* MeshAssetInstance = ObjectCached->GetStaticMeshAssetInstance();

    ImGui::ColorButton("TEST", UIColor::GBlack, NULL, UISize::FileSize);

    SameLine();

    if (ImGui::BeginCombo("Static Mesh Asset", MeshAssetInstance != nullptr ? MeshAssetInstance->GetAssetName().c_str() : "Choose Static Mesh Asset"))
    {
        for (auto& StaticMesh : ManagingStaticMeshes)
        {
            if (ImGui::Selectable(StaticMesh.first.c_str()))
            {
                ObjectCached->SetStaticMeshAssetInstance(StaticMesh.second);
            }
        }

        ImGui::EndCombo();
    }
}
