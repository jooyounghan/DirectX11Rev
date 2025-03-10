#include "ScratchTextureAssetVariableNode.h"

#include "AssetManager.h"
#include "ScratchTextureAsset.h"

#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

ScratchTextureAssetVariableNode::ScratchTextureAssetVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<ScratchTextureAsset*>("Scratch Asset", leftTop, radius, referencedOrigin, {}),
	m_scratchTextureAssetComboBox(format("{}", (uint64_t)this), "", NULL)
{
	AssetManager* assetManager = AssetManager::GetInstance();

	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawImage();
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

	assetManager->RegisterAssetLoadedHandler(
		format("UpdateScratchAssetNode{}", (uint64_t)this), 
		bind(&ScratchTextureAssetVariableNode::UpdateScratchAssetSelectables, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);

	m_scratchTextureAssetComboBox.OnSelChanged = [&, assetManager](const size_t& idx, const string& selectedAssetName)
		{
			m_selectedScratchTextureAsset = assetManager->GetScratchTextureAsset(selectedAssetName);
		};
}

void ScratchTextureAssetVariableNode::DrawImage()
{
	SetNextItemAllowOverlap();

	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultImageSize ? dynamicWidth : defaultImageSize);
	Image((ImU64)(m_selectedScratchTextureAsset == nullptr ? NULL : m_selectedScratchTextureAsset->GetSRV()), ImVec2(dynamicWidth, dynamicWidth));
	ConsumeMouseCaptureForItem();
	PopItemWidth();
}

void ScratchTextureAssetVariableNode::DrawScratchAssetSelectCombo()
{
	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultTextSize ? dynamicWidth : defaultTextSize);
	PushID(format("{}", (uint64_t)this).c_str());
	m_scratchTextureAssetComboBox.Draw();
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}

void ScratchTextureAssetVariableNode::UpdateScratchAssetSelectables(const EAssetType& assetType, std::string s, AAsset* a)
{
	AssetManager* assetManager = AssetManager::GetInstance();
	if (assetType == EAssetType::ASSET_TYPE_SCRATCH_TEXTURE)
	{
		m_scratchTextureAssetComboBox.SetSelectableItems("Select Scratch Texture", assetManager->GetAssetNames(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE));
	}
}

ScratchTextureAsset* ScratchTextureAssetVariableNode::GetVariableImpl(const std::tuple<>& variables)
{
	return m_selectedScratchTextureAsset;
}
