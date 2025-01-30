#include "BaseTextureAssetVariableNode.h"

#include "AssetManager.h"
#include "BaseTextureAsset.h"

#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

BaseTextureAssetVariableNode::BaseTextureAssetVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode<BaseTextureAsset*>("BaseTexture Asset", leftTop, radius, referencedOrigin, {}),
	m_baseTextureAssetComboBox(format("{}", (uint64_t)this), "", NULL)
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
			DrawBaseAssetSelectCombo();
			return GetItemRectWithInternalMargin();
		}
	);

	assetManager->RegisterAssetLoadedHandler(
		format("UpdateBaseTextureAssetNode{}", (uint64_t)this),
		bind(&BaseTextureAssetVariableNode::UpdateBaseAssetSelectables, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);

	m_baseTextureAssetComboBox.OnSelChanged = [&, assetManager](const size_t& idx, const string& selectedAssetName)
		{
			m_selectedBaseTextureAsset = assetManager->GetBaseTextureAsset(selectedAssetName);
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
	m_baseTextureAssetComboBox.Draw();
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}

void BaseTextureAssetVariableNode::UpdateBaseAssetSelectables(const EAssetType& assetType, std::string, AAsset*)
{
	AssetManager* assetManager = AssetManager::GetInstance();
	if (assetType == EAssetType::ASSET_TYPE_BASE_TEXTURE)
	{
		m_baseTextureAssetComboBox.SetSelectableItems("Select Base Texture", assetManager->GetAssetNames(EAssetType::ASSET_TYPE_BASE_TEXTURE));
	}
}

BaseTextureAsset* BaseTextureAssetVariableNode::GetVariableImpl(const std::tuple<>& variables)
{
	return m_selectedBaseTextureAsset;
}
