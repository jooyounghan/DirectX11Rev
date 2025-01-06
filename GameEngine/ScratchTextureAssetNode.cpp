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
	VariableNode<ScratchTextureAsset*>("Scratch Asset", leftTop, radius, referencedOrigin),
	m_scratchTextureAssetComboBox(format("{}", (uint64_t)this), "", NULL)
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawDrawImage();
			return GetItemRectWithInternalMargin();
		}
	);
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawScratchAssetSelectCombo();
			return GetItemRectWithInternalMargin();
		}
	);

	m_assetMangerCached->RegisterAssetLoadedHandler(
		format("UpdateScratchAssetNode{}", (uint64_t)this), 
		bind(&ScratchTextureAssetNode::UpdateScratchAssetSelectables, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);

	m_scratchTextureAssetComboBox.OnSelChanged = [&](const size_t& idx, const string& selectedAssetName) 
		{
			m_selectedScratchTextureAsset = m_assetMangerCached->GetScratchTextureAsset(selectedAssetName);
		};
}

void ScratchTextureAssetNode::DrawDrawImage()
{
	SetNextItemAllowOverlap();

	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultImageSize ? dynamicWidth : defaultImageSize);
	Image((ImU64)(m_selectedScratchTextureAsset == nullptr ? NULL : m_selectedScratchTextureAsset->GetSRV()), ImVec2(dynamicWidth, dynamicWidth));
	ConsumeMouseCaptureForItem();
	PopItemWidth();
}

void ScratchTextureAssetNode::DrawScratchAssetSelectCombo()
{
	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultTextSize ? dynamicWidth : defaultTextSize);
	PushID(format("{}", (uint64_t)this).c_str());
	m_scratchTextureAssetComboBox.Draw();
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}

void ScratchTextureAssetNode::UpdateScratchAssetSelectables(const EAssetType& assetType, std::string s, AAsset* a)
{
	if (assetType == EAssetType::ASSET_TYPE_SCRATCH_TEXTURE)
	{
		m_scratchTextureAssetComboBox.SetSelectableItems(m_assetMangerCached->GetAssetNames(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE));
	}
}

ScratchTextureAsset* ScratchTextureAssetNode::GetVariableImpl(const std::tuple<>& variables)
{
	return m_selectedScratchTextureAsset;
}
