#include "SkeletalMeshObjectInformationDrawer.h"
#include "SkeletalMeshObject.h"
#include "SkeletalMeshAsset.h"

#include "AssetManager.h"

#include "UIVariable.h"

#include "AssetSelectHelper.h"

// Test
#include "AnimationPlayer.h"
#include "AnimationAsset.h"



using namespace std;
using namespace ImGui;

SkeletalMeshObjectInformationDrawer::SkeletalMeshObjectInformationDrawer(SkeletalMeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{
}

void SkeletalMeshObjectInformationDrawer::DrawInformation()
{
    SeparatorText("Skeletal Mesh Object");

    const unordered_map<string, shared_ptr<SkeletalMeshAsset>>& ManagingSkeletalMeshes = AssetManagerCached->GetManagingSkeletalMeshes();
    SkeletalMeshAsset* const MeshAssetInstance = ObjectCached->GetSkeletalMeshAssetInstance();

    Image(nullptr, UISize::FileSize);

    SameLine();

    shared_ptr<SkeletalMeshAsset> SkeletalMeshResult = AssetSelectHelper::SelectAsset(
        ManagingSkeletalMeshes, MeshAssetInstance, 
        "SelectSkeletalMeshAssetFromSkeletalMesh",
        "Skeletal Mesh Asset", "Choose Skeletal Mesh Asset"
    );
    if (SkeletalMeshResult != nullptr) ObjectCached->SetSkeletalMeshAssetInstance(SkeletalMeshResult);

    // Test
    SeparatorText("Animation");

    const unordered_map<string, shared_ptr<AnimationAsset>>& ManagingAnimations = AssetManagerCached->GetManagingAnimations();
    const AnimationPlayer& AnimPlayer = ObjectCached->GetAnimationPlayerInstance();
    AnimationAsset* const AnimAssetCached = AnimPlayer.GetAnimationAssetCached();
    shared_ptr<AnimationAsset> AnimResult = AssetSelectHelper::SelectAsset(
        ManagingAnimations, AnimAssetCached,
        "SelectAnimationAssetFromSkeletalMesh",
        "Animation Asset", "Choose Animation Asset"
    );
    if (AnimResult != nullptr) ObjectCached->SetAnimationAssetInstance(AnimResult);
}
