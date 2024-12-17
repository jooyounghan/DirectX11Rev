#include "AssetFileControl.h"

using namespace std;
using namespace ImGui;

AssetFileControl::AssetFileControl(const EAssetType& assetType, AAsset* asset, ID3D11ShaderResourceView* thumbnailSRV)
	: m_assetType(assetType), m_assetCached(asset), m_thumbanilCached(thumbnailSRV)
{
}

void AssetFileControl::RenderControlImpl()
{
	constexpr float yMargin = 5;
	constexpr float controlWidth = 100.f;
	constexpr float controlHeight = 100.f;
	constexpr ImVec4 hilightedColor = ImVec4(0.2f, 0.3f, 0.4f, 1.0f);

	const string& assetName = m_assetCached->GetAssetName();

	ImVec2 textSize = ImGui::CalcTextSize(assetName.c_str(), nullptr, true, controlWidth);
	ImVec2 childSize = ImVec2(controlWidth, controlHeight + textSize.y + yMargin);

	PushID(assetName.c_str());
	
	if (m_isHilighted)
	{
		PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, hilightedColor);
		BeginChild("", childSize, NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
	}

	BeginGroup();
	Image(m_thumbanilCached, ImVec2(controlWidth, controlWidth));
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + controlWidth);
	ImGui::Text(assetName.c_str());
	ImGui::PopTextWrapPos();
	EndGroup();

	if (m_isHilighted)
	{
		EndChild();
		ImGui::PopStyleColor();
	}

	PopID();

	const ImVec2 rectSize = GetItemRectSize();
	m_width = rectSize.x;
	m_height = rectSize.y;

}

bool AssetFileControl::IsPointIn(const float& pointX, const float& pointY) const 
{
	if (m_left <= pointX && pointX <= m_left + m_width)
	{
		if (m_top <= pointY && pointY <= m_top + m_height)
		{
			return true;
		}
	}
	return false;
}

void AssetFileControl::OnMouseEnter(MouseEventArgs& args)
{
	m_isHilighted = true;
}

void AssetFileControl::OnMouseLeave(MouseEventArgs& args)
{
	m_isHilighted = false;
}

void AssetFileControl::OnBeginDrag()
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_::ImGuiDragDropFlags_SourceExtern))
	{
		const string& AssetName = m_assetCached->GetAssetName();
		ImGui::SetDragDropPayload("DND_ASSET", this, sizeof(AssetFileControl*));
		ImGui::Text(AssetName.c_str());
		ImGui::EndDragDropSource();
	}
}
