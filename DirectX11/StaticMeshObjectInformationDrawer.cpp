#include "StaticMeshObjectInformationDrawer.h"
#include "StaticMeshObject.h"
#include "StaticMeshAsset.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "AssetSelectHelper.h"

#include "UIVariable.h"

using namespace std;
using namespace ImGui;

StaticMeshObjectInformationDrawer::StaticMeshObjectInformationDrawer(StaticMeshObject* ObjectIn)
    : AInformationDrawer(ObjectIn)
{
}

void StaticMeshObjectInformationDrawer::DrawInformation()
{
	SeparatorText("Static Mesh Object");

    const std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>>& ManagingStaticMeshes = App::GAssetManager->GetManagingStaticMeshes();

    StaticMeshAsset* const MeshAssetInstance = ObjectCached->GetStaticMeshAssetInstance();

    Image(nullptr, UISize::FileSize);

    SameLine();

    shared_ptr<StaticMeshAsset> Result = AssetSelectHelper::SelectAsset(ManagingStaticMeshes, MeshAssetInstance,
        "SelectSkeletalMeshAssetFromStaticMesh", "Static Mesh", "Select Static Mesh Asset");

    if (Result != nullptr) ObjectCached->SetStaticMeshAssetInstance(Result);
}
