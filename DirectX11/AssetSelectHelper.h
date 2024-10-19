#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "AAssetFile.h"
#include "StaticAssertHelper.h"

#include <memory>
#include <unordered_map>
#include <string>

class AssetSelectHelper
{
public:
	template<typename AssetType>
	static std::shared_ptr<AssetType> SelectAsset(
		const std::unordered_map<std::string, std::shared_ptr<AssetType>>& ManagingAssetsIn,
		AssetType* CurrentSelected, const std::string& SelectUIID, const char* StrId, const char* PreviewText,
        const ImGuiSelectableFlags& SelectableFlag = NULL,
        const float& ItemWidthIn = 0.f
	);
};

template<typename AssetType>
inline std::shared_ptr<AssetType> AssetSelectHelper::SelectAsset(
    const std::unordered_map<std::string, std::shared_ptr<AssetType>>& ManagingAssetsIn,
    AssetType* CurrentSelected, const std::string& SelectUIID, const char* DescriptionId, const char* PreviewText,
    const ImGuiSelectableFlags& SelectableFlag,
    const float& ItemWidthIn
)
{
    static_assert(std::is_base_of<AAssetFile, AssetType>::value, DerivedCondition(AAssetFile));

    std::shared_ptr<AssetType> Result = nullptr;

    ImGui::PushItemWidth(ItemWidthIn);
    ImGui::BeginGroup();
    {
        ImGui::Text(DescriptionId);
        ImGui::PushID(SelectUIID.c_str());
        {
            if (ImGui::BeginCombo("", CurrentSelected != nullptr ? CurrentSelected->GetAssetName().c_str() : PreviewText))
            {
                for (auto& ManagingAsset : ManagingAssetsIn)
                {
                    if (ImGui::Selectable(ManagingAsset.first.c_str()))
                    {
                        Result = ManagingAsset.second;
                    }
                }

                ImGui::EndCombo();
            }
        }
        ImGui::PopID();
    }
    ImGui::EndGroup();
    ImGui::PopItemWidth();
    return Result;
}
