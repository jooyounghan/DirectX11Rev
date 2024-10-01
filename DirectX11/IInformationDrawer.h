#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include <memory>
#include <unordered_map>
#include <string>

class IInformationDrawerImp
{
public:
	virtual void DrawInformation() = 0;
};

template<typename T>
class AInformationDrawer : public IInformationDrawerImp
{
public:
	AInformationDrawer(T* ObjectIn);

protected:
	T* ObjectCached = nullptr;

protected:
	template<typename AssetType>
	std::shared_ptr<AssetType> SelectAsset(
		const std::unordered_map<std::string, std::shared_ptr<AssetType>>& ManagingAssetsIn,
		AssetType* CurrentSelected, const char* StrId, const char* PreviewText
	);
};

template<typename T>
inline AInformationDrawer<T>::AInformationDrawer(T* ObjectIn)
    : ObjectCached(ObjectIn) {}


template<typename T>
template<typename AssetType>
inline std::shared_ptr<AssetType> AInformationDrawer<T>::SelectAsset(
    const std::unordered_map<std::string, std::shared_ptr<AssetType>>& ManagingAssetsIn,
    AssetType* CurrentSelected, const char* StrId, const char* PreviewText
)
{
    std::shared_ptr<AssetType> Result = nullptr;
    ImGui::BeginGroup();
    {
        ImGui::Text(StrId);
        ImGui::PushID(PreviewText);
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
    return Result;
}

