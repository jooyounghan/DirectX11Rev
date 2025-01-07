#include "BaseTextureAssetVariableNode.h"

#include "AssetManager.h"
#include "BaseTextureAsset.h"

#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

BaseTextureAssetVariableNode::BaseTextureAssetVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin, AssetManager* assetManager)
	: m_assetMangerCached(assetManager),
	VariableNode<BaseTextureAsset*>("BaseTexture Asset", leftTop, radius, referencedOrigin, {}),
	m_scratchTextureAssetComboBox(format("{}", (uint64_t)this), "", NULL)
{
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
			DrawBaseAssetSelectCombo();
			return GetItemRectWithInternalMargin();
		}
	);

	m_assetMangerCached->RegisterAssetLoadedHandler(
		format("UpdateScratchAssetNode{}", (uint64_t)this),
		bind(&BaseTextureAssetVariableNode::UpdateScratchAssetSelectables, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);

	m_scratchTextureAssetComboBox.OnSelChanged = [&](const size_t& idx, const string& selectedAssetName)
		{
			m_selectedBaseTextureAsset = m_assetMangerCached->GetBaseTextureAsset(selectedAssetName);
		};
}


void BaseTextureAssetVariableNode::DrawImage()
{
	SetNextItemAllowOverlap();

	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultImageSize ? dynamicWidth : defaultImageSize);
	Image((ImU64)(m_selectedBaseTextureAsset == nullptr ? NULL : m_selectedBaseTextureAsset->GetSRV()), ImVec2(dynamicWidth, dynamicWidth));
	ConsumeMouseCaptureForItem();
	PopItemWidth();
}

void BaseTextureAssetVariableNode::DrawBaseAssetSelectCombo()
{
	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultTextSize ? dynamicWidth : defaultTextSize);
	PushID(format("{}", (uint64_t)this).c_str());
	m_scratchTextureAssetComboBox.Draw();
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}

void BaseTextureAssetVariableNode::UpdateScratchAssetSelectables(const EAssetType& assetType, std::string, AAsset*)
{
	if (assetType == EAssetType::ASSET_TYPE_SCRATCH_TEXTURE)
	{
		m_scratchTextureAssetComboBox.SetSelectableItems(m_assetMangerCached->GetAssetNames(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE));
	}
}

BaseTextureAsset* BaseTextureAssetVariableNode::GetVariableImpl(const std::tuple<>& variables)
{
	return m_selectedBaseTextureAsset;
}
