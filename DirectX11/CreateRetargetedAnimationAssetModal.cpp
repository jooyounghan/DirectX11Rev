#include "CreateRetargetedAnimationAssetModal.h"
#include "AssetManager.h"
#include "AssetSelectHelper.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;
using namespace ImGui;

CreateRetargetedAnimationAssetModal::CreateRetargetedAnimationAssetModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn)
    : CreateAssetModal(ModalHeaderNameIn, AssetManagerIn)
{
}

CreateRetargetedAnimationAssetModal::~CreateRetargetedAnimationAssetModal()
{
}

void CreateRetargetedAnimationAssetModal::RenderModal()
{
    CreateAssetModal::RenderModal();

    const unordered_map<string, shared_ptr<BoneAsset>>& ManagingBones = AssetManagerCached->GetManagingBones();
    const unordered_map<string, shared_ptr<AnimationAsset>>& ManagingAnimations = AssetManagerCached->GetManagingAnimations();

    BoneAsset* const FromBoneAsset = AnimationRetargeterInstance.GetFromBoneAsset();
    BoneAsset* const ToBoneAsset = AnimationRetargeterInstance.GetToBoneAsset();
    const map<string, Bone>& NameToFromBones = FromBoneAsset->GetNameToBones();
    const unordered_map<string, string>& BoneTargetings =  AnimationRetargeterInstance.GetBoneTargetings();
    AnimationAsset* const TargetAnimationAsset = AnimationRetargeterInstance.GetTargetAnimationAsset();

    shared_ptr<AnimationAsset> TargetAnimationSelected = AssetSelectHelper::SelectAsset(
        ManagingAnimations, TargetAnimationAsset,
        "SelectTargetAnimationAsset",
        "Target Animation Asset", "Select Target Animation Asset"
    );
    shared_ptr<BoneAsset> FromBoneSelected = AssetSelectHelper::SelectAsset(
        ManagingBones, FromBoneAsset,
        "SelectFromBoneAsset",
        "From Bone Asset", "Select From Bone Asset"
    );
    SameLine();
    shared_ptr<BoneAsset> ToBoneSelected = AssetSelectHelper::SelectAsset(
        ManagingBones, ToBoneAsset,
        "SelectToBoneAsset",
        "To Bone Asset", "Select To Bone Asset"
    );

    if (BeginTable(
        "Retarget Bone Asset", 2, 
        ImGuiTableFlags_::ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_::ImGuiTableFlags_ScrollY | 
        ImGuiTableFlags_::ImGuiTableFlags_SizingStretchProp
    ))
    {
        TableSetupColumn("To Bone");
        TableSetupColumn("From Bone");
        ImGui::TableHeadersRow();

        for (auto& BoneTargeting : BoneTargetings)
        {
            ImGui::TableNextRow();
            const string& ToBoneName = BoneTargeting.first;
            const string& FromBoneName = BoneTargeting.second;

            ImGui::TableSetColumnIndex(0);
            Text(ToBoneName.c_str());

            ImGui::TableSetColumnIndex(1);
            PushID(ToBoneName.c_str());
            if (ImGui::BeginCombo("", FromBoneName.c_str()))
            {
                if (ImGui::Selectable("Empty"))
                {
                    AnimationRetargeterInstance.ReplaceTargetedFromBone(ToBoneName, "");
                }
                for (auto& NameToFromBone : NameToFromBones)
                {
                    if (ImGui::Selectable(NameToFromBone.first.c_str()))
                    {
                        AnimationRetargeterInstance.ReplaceTargetedFromBone(ToBoneName, NameToFromBone.first);
                    }
                }

                ImGui::EndCombo();
            }
            PopID();
        }

        EndTable();
    }

    bool NeedRegenerateTargeting = false;

    if (FromBoneSelected != nullptr) { AnimationRetargeterInstance.SetFromBoneAsset(FromBoneSelected); NeedRegenerateTargeting = true; }
    if (ToBoneSelected != nullptr) { AnimationRetargeterInstance.SetToBoneAsset(ToBoneSelected); NeedRegenerateTargeting = true; }
    if (TargetAnimationSelected != nullptr) { AnimationRetargeterInstance.SetTargetAnimationAsset(TargetAnimationSelected); NeedRegenerateTargeting = true; }

    if (NeedRegenerateTargeting) AnimationRetargeterInstance.GenerateBoneTargetings();


    ImGui::BeginDisabled(IsNotValidFileName || BoneTargetings.empty());
    if (Button("OK", ImVec2(120, 0)))
    {
        shared_ptr<AnimationAsset> RetargetedAnimationAsset = AnimationRetargeterInstance.GetRetargetedAnimation(AssetNameBuffer);
        if (RetargetedAnimationAsset != nullptr)
        {
            AssetManagerCached->SerailizeAndAddToContainer(RetargetedAnimationAsset);
            CloseCurrentPopup();
        }
    }
    SetItemDefaultFocus();
    ImGui::EndDisabled();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0)))
    {
        CloseCurrentPopup();
    }
}
