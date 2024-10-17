#include "SkeletalMeshObjectInformationDrawer.h"
#include "SkeletalMeshObject.h"
#include "SkeletalMeshAsset.h"

#include "AssetManager.h"

#include "UIVariable.h"

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

    SkeletalMeshAsset* const MeshAssetInstance = ObjectCached->GetSkeletalMeshAssetInstance();

    Image(nullptr, UISize::FileSize);

    SameLine();

    shared_ptr<SkeletalMeshAsset> Result = SelectAsset(ManagingSkeletalMeshes, MeshAssetInstance, "Skeletal Mesh Asset", "Choose Skeletal Mesh Asset");
    if (Result != nullptr) ObjectCached->SetSkeletalMeshAssetInstance(Result);

}
