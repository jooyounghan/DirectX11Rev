#include "StaticMeshObjectInformationDrawer.h"
#include "StaticMeshObject.h"
#include "StaticMeshAsset.h"

#include "AssetManager.h"
#include "AssetSelectHelper.h"

#include "UIVariable.h"

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

    StaticMeshAsset* const MeshAssetInstance = ObjectCached->GetStaticMeshAssetInstance();

    Image(nullptr, UISize::FileSize);

    SameLine();

    shared_ptr<StaticMeshAsset> Result = AssetSelectHelper::SelectAsset(ManagingStaticMeshes, MeshAssetInstance,
        "SelectSkeletalMeshAssetFromStaticMesh", "Static Mesh", "Select Static Mesh Asset");

    if (Result != nullptr) ObjectCached->SetStaticMeshAssetInstance(Result);
}
