#include "CreateRetargetedAnimationAssetModal.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "AssetSelectHelper.h"

#include "BoneAsset.h"
#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;
using namespace ImGui;

CreateRetargetedAnimationAssetModal::CreateRetargetedAnimationAssetModal(const std::string& ModalHeaderNameIn)
    : CreateAssetModal(ModalHeaderNameIn)
{
}

CreateRetargetedAnimationAssetModal::~CreateRetargetedAnimationAssetModal()
{
}

void CreateRetargetedAnimationAssetModal::RenderModal()
{
    CreateAssetModal::RenderModal();

    const unordered_map<string, shared_ptr<BoneAsset>>& ManagingBones = App::GAssetManager->GetManagingBones();
    const unordered_map<string, shared_ptr<AnimationAsset>>& ManagingAnimations = App::GAssetManager->GetManagingAnimations();

    BoneAsset* const SourceBoneAsset = AnimationRetargeterInstance.GetSourceBoneAsset();
    BoneAsset* const ToBoneAsset = AnimationRetargeterInstance.GetDestBoneAsset();
    const map<string, Bone>& NameToSourceBones = SourceBoneAsset->GetNameToBones();
    const unordered_map<string, string>& BoneTargetings =  AnimationRetargeterInstance.GetBoneTargetings();

    AnimationAsset* const SourceAnimationAsset = AnimationRetargeterInstance.GetSourceAnimationAsset();

    shared_ptr<AnimationAsset> SourceAnimationSelected = AssetSelectHelper::SelectAsset(
        ManagingAnimations, SourceAnimationAsset,
        "SelectSourceAnimationAsset",
        "Source Animation Asset", "Select Source Animation Asset"
    );

    // Bone 
    shared_ptr<BoneAsset> SourceBoneSelected = AssetSelectHelper::SelectAsset(
        ManagingBones, SourceBoneAsset,
        "SelectSourceBoneAsset",
        "Source Bone Asset", "Select Source Bone Asset"
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
        TableSetupColumn("Source Bone");
        ImGui::TableHeadersRow();

        for (auto& BoneTargeting : BoneTargetings)
        {
            ImGui::TableNextRow();
            const string& ToBoneName = BoneTargeting.first;
            const string& SourceBoneName = BoneTargeting.second;

            ImGui::TableSetColumnIndex(0);
            Text(ToBoneName.c_str());

            ImGui::TableSetColumnIndex(1);
            PushID(ToBoneName.c_str());
            if (ImGui::BeginCombo("", SourceBoneName.c_str()))
            {
                if (ImGui::Selectable("Empty"))
                {
                    AnimationRetargeterInstance.ReplaceTargetedSourceBone(ToBoneName, "");
                }
                for (auto& NameToSourceBone : NameToSourceBones)
                {
                    if (ImGui::Selectable(NameToSourceBone.first.c_str()))
                    {
                        AnimationRetargeterInstance.ReplaceTargetedSourceBone(ToBoneName, NameToSourceBone.first);
                    }
                }

                ImGui::EndCombo();
            }
            PopID();
        }

        EndTable();
    }

    bool NeedRegenerateTargeting = false;

    if (SourceBoneSelected != nullptr) { AnimationRetargeterInstance.SetSourceBoneAsset(SourceBoneSelected); NeedRegenerateTargeting = true; }
    if (ToBoneSelected != nullptr) { AnimationRetargeterInstance.SetDestBoneAsset(ToBoneSelected); NeedRegenerateTargeting = true; }
    
    if (SourceAnimationSelected != nullptr) { AnimationRetargeterInstance.SetSourceAnimationAsset(SourceAnimationSelected); }

    if (NeedRegenerateTargeting) AnimationRetargeterInstance.GenerateBoneTargetings();


    ImGui::BeginDisabled(IsNotValidFileName || BoneTargetings.empty());
    if (Button("OK", ImVec2(120, 0)))
    {
        shared_ptr<AnimationAsset> RetargetedAnimationAsset = AnimationRetargeterInstance.GetRetargetedAnimation(AssetNameBuffer);
        if (RetargetedAnimationAsset != nullptr)
        {
            App::GAssetManager->SerailizeAndAddToContainer(RetargetedAnimationAsset);
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
