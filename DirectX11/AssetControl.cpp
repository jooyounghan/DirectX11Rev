#include "AssetControl.h"
#include "AAssetFile.h"

#include "BaseTextureAsset.h"
#include "DDSTextureAsset.h"
#include "EXRTextureAsset.h"

#include "UIVariable.h"

using namespace ImGui;
using namespace std;

AssetControl::AssetControl(AAssetFile* AssetFileIn)
	: AssetFileCached(AssetFileIn)
{
}

AssetControl::~AssetControl()
{
}

void AssetControl::RenderControl()
{
    if (AssetFileCached != nullptr)
    {
        const string& AssetName = AssetFileCached->GetAssetName();
        const string& AssetType = AssetFileCached->GetAssetType();

        ImGui::PushID(AssetName.c_str());

        if (bIsMouseHovering || bIsFocused)
        {
            StartHilightedEvent.Invoke(this);
        }

        ImGui::BeginGroup();
        if (AssetType == BaseTextureAsset::BaseTextureAssetKind || AssetType == EXRTextureAsset::EXRTextureAssetKind)
        {
            Image(AssetFileCached->GetThumbnailSRV(), UISize::FileSize);
        }
        else if (AssetType == DDSTextureAsset::DDSTextureAssetKind)
        {
            ID3D11ShaderResourceView* SRV = AssetFileCached->GetThumbnailSRV();
            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
            SRV->GetDesc(&SRVDesc);
            if (SRVDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
            {
                Image(SRV, UISize::FileSize);
            }
            else
            {
                Image(nullptr, UISize::FileSize);
            }
        }
        else
        {
            Image(nullptr, UISize::FileSize);
        }

        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + UISize::FileSize.x);
        ImGui::Text(AssetName.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndGroup();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_::ImGuiDragDropFlags_SourceAllowNullID))
        {
            BeginDragDropEvent.Invoke(AssetFileCached);
        }

        if (bIsMouseHovering || bIsFocused)
        {
            EndHilightedEvent.Invoke(this);
        }

        ImGui::PopID();

        if (IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
        {

            bIsMouseHovering = true;

            if (IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                LeftMouseClickedEvent.Invoke(this);
            }
            
            if (IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                LeftMouseDBClickedEvent.Invoke(this);
            }
        }
        else
        {
            bIsMouseHovering = false;
        }
    }
}
