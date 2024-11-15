#include "SkeletalMeshObjectInformationDrawer.h"
#include "SkeletalMeshObject.h"
#include "SkeletalMeshAsset.h"

#include "GlobalVariable.h"
#include "AssetManager.h"

#include "UIVariable.h"

#include "AssetSelectHelper.h"

// Test
#include "AnimationPlayer.h"
#include "AnimationAsset.h"



using namespace std;
using namespace ImGui;

SkeletalMeshObjectInformationDrawer::SkeletalMeshObjectInformationDrawer(SkeletalMeshObject* ObjectIn)
    : AInformationDrawer(ObjectIn)
{
}

void SkeletalMeshObjectInformationDrawer::DrawInformation()
{
    SeparatorText("Skeletal Mesh Object");

    const unordered_map<string, shared_ptr<SkeletalMeshAsset>>& ManagingSkeletalMeshes = App::GAssetManager->GetManagingSkeletalMeshes();
    SkeletalMeshAsset* const MeshAssetInstance = ObjectCached->GetSkeletalMeshAssetInstance();

    Image(nullptr, UISize::FileSize);

    SameLine();

    shared_ptr<SkeletalMeshAsset> SkeletalMeshResult = AssetSelectHelper::SelectAsset(
        ManagingSkeletalMeshes, MeshAssetInstance, 
        "SelectSkeletalMeshAssetFromSkeletalMesh",
        "Skeletal Mesh Asset", "Select Skeletal Mesh Asset"
    );
    if (SkeletalMeshResult != nullptr) ObjectCached->SetSkeletalMeshAssetInstance(SkeletalMeshResult);

    // Test
    SeparatorText("Animation");

    const unordered_map<string, shared_ptr<AnimationAsset>>& ManagingAnimations = App::GAssetManager->GetManagingAnimations();
    const AnimationPlayer& AnimPlayer = ObjectCached->GetAnimationPlayerInstance();
    AnimationAsset* const AnimAssetCached = AnimPlayer.GetAnimationAssetCached();
    shared_ptr<AnimationAsset> AnimResult = AssetSelectHelper::SelectAsset(
        ManagingAnimations, AnimAssetCached,
        "SelectAnimationAssetFromSkeletalMesh",
        "Animation Asset", "Select Animation Asset"
    );
    if (AnimResult != nullptr) ObjectCached->SetAnimationAssetInstance(AnimResult);
}
