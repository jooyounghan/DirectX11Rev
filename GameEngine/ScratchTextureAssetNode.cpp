#include "ScratchTextureAssetNode.h"

#include "AssetManager.h"
#include "ScratchTextureAsset.h"

#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

ScratchTextureAssetNode::ScratchTextureAssetNode(
	const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin, AssetManager* assetManager
)
	: m_assetMangerCached(assetManager), 
	VariableNode<ScratchTextureAsset*>("Scratch Asset", leftTop, radius, referencedOrigin)
{

}
//
//void ScratchTextureAssetNode::DrawImpl(ImDrawList* drawListIn)
//{
//	VariableNode<ScratchTextureAsset*>::DrawImpl(drawListIn);
//
//	const ImVec2& drawNodeFieldPos = GetDrawNodeFieldPos();
//	const ImVec2& drawNodeFieldSize = GetDrawNodeFieldSize();
//	ImVec2 drawMarginedFieldPos = ImVec2(drawNodeFieldPos.x + nodeInternalMargin, drawNodeFieldPos.y + nodeInternalMargin);
//	SetCursorScreenPos(drawMarginedFieldPos);
//
//	drawListIn->AddCircle(drawMarginedFieldPos, 5.f, IM_COL32(255, 0, 0, 255));
//
//	const float imageSize = drawNodeFieldSize.x - 2.f * nodeInternalMargin;
//	m_imageSize = ImVec2(imageSize, imageSize);
//
//	PushID(format("{}", (uint64_t)this).c_str());
//	SetNextItemAllowOverlap();
//	Image((ImU64)(m_selectedScratchTextureAsset == nullptr ? NULL : m_selectedScratchTextureAsset->GetSRV()), m_imageSize);
//	drawMarginedFieldPos.y += imageSize;
//	SetCursorScreenPos(drawMarginedFieldPos);
//	drawListIn->AddCircle(drawMarginedFieldPos, 5.f, IM_COL32(255, 0, 0, 255));
//
//	Text("Testasdasdasdasdsad");
//
//	ImGuiIO& io = ImGui::GetIO();
//	if (IsItemActive()) io.WantCaptureMouse = false;
//	m_comboSize = GetItemRectSize();
//
//	PopID();
//}


ScratchTextureAsset* ScratchTextureAssetNode::GetVariableImpl(const std::tuple<>& variables)
{
	return nullptr;
}
