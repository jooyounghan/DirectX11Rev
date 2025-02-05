#include "CreateAnimationRetargeterWindow.h"
#include "AssetManager.h"
#include "AssetWriter.h"

#include <format>
#include <algorithm>

using namespace std;
using namespace ImGui;

CreateAnimationRetargeterWindow::CreateAnimationRetargeterWindow(
    ID3D11Device* const* deviceAddress, 
    ID3D11DeviceContext* const* deviceContextAddress, 
    const std::string& windowID, 
    bool* openFlag
)
    : AWindow(windowID, false, openFlag, ImGuiWindowFlags_NoDocking),
    m_deviceAddress(deviceAddress),
    m_deviceContextAddress(deviceContextAddress),
    m_boneSourceComboBox("boneSourceCombo", "Select Source Bone"),
    m_boneTargetComboBox("boneTargetCombo", "Select Target Bone"),
    m_animationSourceComboBox("animationSourceCombo", "Select Source Animation")
{
    m_boneSourceComboBox.OnSelChanged = [&](const size_t&, const string& sourceBoneName) { m_selectedBoneSourceName = sourceBoneName; };
    m_boneTargetComboBox.OnSelChanged = [&](const size_t&, const string& targetBoneName) { m_selectedBoneTargetName = targetBoneName; };
    m_animationSourceComboBox.OnSelChanged = [&](const size_t&, const string& sourceAnimationName) { m_selectedAnimationSourceName = sourceAnimationName; };
}


void CreateAnimationRetargeterWindow::RenderWindowImpl()
{
    static string selectedBoneSourceName;
    static string selectedBoneTargetName;
    static string selectedAnimationSourceName;
    static vector<string> targetBoneNames;
    static map<string, Bone*> targetBoneNameToBones;
    static char buffer[1024];

    AssetManager* assetManager = AssetManager::GetInstance();
    const vector<string> boneAssetNames = assetManager->GetAssetNames(EAssetType::ASSET_TYPE_BONE);
    const vector<string> animationAssetNames = assetManager->GetAssetNames(EAssetType::ASSET_TYPE_ANIMATION);   

    m_boneSourceComboBox.SetSelectableItems("", boneAssetNames);
    m_boneTargetComboBox.SetSelectableItems("", boneAssetNames);
    m_animationSourceComboBox.SetSelectableItems("", animationAssetNames);

    InputText("Retargeted Animation Name", buffer, 1024);

    bool isChanged = false;

    if (m_boneSourceComboBox.Draw())
    {
        m_animationRetargeter.SetSourceBoneAsset(assetManager->GetBoneAsset(m_selectedBoneSourceName));
        isChanged = true;
    }
    if (m_boneTargetComboBox.Draw())
    {
        const BoneAsset* targetBoneAsset = assetManager->GetBoneAsset(m_selectedBoneTargetName);
        m_animationRetargeter.SetTargetBoneAsset(targetBoneAsset);

        targetBoneNames.clear();
        targetBoneNameToBones.clear();

        const vector<Bone*>& targetBones = targetBoneAsset->GetBones();
        for (Bone* targetBone : targetBones)
        {
            const string& targetBoneName = targetBone->GetBoneName();
            targetBoneNames.emplace_back(targetBoneName);
            targetBoneNameToBones.emplace(targetBoneName, targetBone);
        }
        isChanged = true;
    }
    if (m_animationSourceComboBox.Draw())
    {
        m_animationRetargeter.SetSourceAnimationAsset(assetManager->GetAnimationAsset(m_selectedAnimationSourceName));
        isChanged = true;
    }
    if (isChanged)
    {
        m_animationRetargeter.GenerateBoneTargetings();
    }

    const vector<pair<const Bone*, const Bone*>>& targetedBones = m_animationRetargeter.GetBoneTargetings();
    
    ImVec2 regionAvailSize = GetContentRegionAvail();
    if (BeginTable("Retarget Bone Asset", 2,
        ImGuiTableFlags_::ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_::ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_::ImGuiTableFlags_SizingStretchProp, ImVec2(regionAvailSize.x, regionAvailSize.y * 0.9f))
    )
    {
        TableSetupColumn("Source Bone");
        TableSetupColumn("Target Bone");
        ImGui::TableHeadersRow();

        for (size_t idx = 0; idx < targetedBones.size(); ++ idx)
        {
            auto& targetedBone = targetedBones[idx];

            ImGui::TableNextRow();
            const Bone* sourceBone = targetedBone.first;
            const Bone* targetBone = targetedBone.second;

            const string sourceBoneName = sourceBone ? sourceBone->GetBoneName() : "";
            const string targetBoneName = targetBone ? targetBone->GetBoneName() : "";
            std::string boneTargetingID;
            boneTargetingID.reserve(sourceBoneName.size() + targetBoneName.size() + 1);
            boneTargetingID = sourceBoneName + "_" + targetBoneName;

            ImGui::TableSetColumnIndex(0);
            Text(sourceBoneName.c_str());

            ImGui::TableSetColumnIndex(1);

            string replacedTargetBoneName;
            ImGuiComboBox targetBoneCombo(boneTargetingID, "");
            targetBoneCombo.OnSelChanged = [&](const size_t&, const string& replacedTargetBoneName_) { replacedTargetBoneName = replacedTargetBoneName_; };
            targetBoneCombo.SetSelectableItems(targetBoneName, targetBoneNames);
            if (targetBoneCombo.Draw())
            {
                m_animationRetargeter.ReplaceTargetBone(idx, sourceBone, targetBoneNameToBones.at(replacedTargetBoneName));
            }
        }
        EndTable();
    }

    bool isAvailableRetargeting = m_animationRetargeter.IsAvailableRetargeting();

    ImGui::BeginDisabled(!isAvailableRetargeting);
    if (Button("Create Retargeted Animation"))
    {
        string retargetedAnimationName = string(buffer);
        AnimationAsset* retargetedAnimationAsset = m_animationRetargeter.GetRetargetedAnimation(retargetedAnimationName);
        if (retargetedAnimationAsset != nullptr)
        {
            assetManager->AddAssetHelper(
                *m_deviceAddress, *m_deviceContextAddress, 
                EAssetType::ASSET_TYPE_ANIMATION, 
                "./Asset/" + retargetedAnimationName, retargetedAnimationAsset
            );
            AAssetWriter::SaveAssets("./Assets", EAssetType::ASSET_TYPE_ANIMATION, { retargetedAnimationAsset });
            *m_openFlag = false;
        }
    }
    SetItemDefaultFocus();
    ImGui::EndDisabled();
    SameLine();
    if (Button("Cancel"))
    {
        *m_openFlag = false;
    }
}
