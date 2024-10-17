#include "AssetControl.h"
#include "AAssetFile.h"

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
        EAssetType AssetType = AssetFileCached->GetAssetType();

        ImGui::PushID(AssetName.c_str());
        ImGui::BeginGroup();

        if (bIsMouseHovering || bIsFocused)
        {
            HilightedEvent.Invoke(this);
        }

        switch (AssetType)
        {
        case EAssetType::BasicTexture:
        case EAssetType::EXRTexture:
            Image(AssetFileCached->GetThumbnailSRV(), UISize::FileSize);
            break;
        case EAssetType::DDSTexture:
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
            break;
        }
        default:
            Image(nullptr, UISize::FileSize);
            break;
        }

        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + UISize::FileSize.x);
        ImGui::Text(AssetName.c_str());
        ImGui::PopTextWrapPos();

        if (bIsMouseHovering || bIsFocused)
        {
            UnhilightedEvent.Invoke(this);
        }

        ImGui::EndGroup();
        ImGui::PopID();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            BeginDragDropEvent.Invoke(AssetFileCached);
        }

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
