#include "AssetWriterPathNode.h"

#include "AssetManager.h"

#include "NodeConstant.h"

#include <format>

using namespace std;
using namespace ImGui;

AssetWriterPathNode::AssetWriterPathNode(
	const ImVec2& leftTop, const float& radius, 
	const ImVec2& referencedOrigin, AssetManager* assetManager
)
	: m_assetMangerCached(assetManager),
	VariableNode<string>("Writer Asset Path", leftTop, radius, referencedOrigin, {}),
	m_assetWriterPathComboBox(format("{}", (uint64_t)this), "", NULL)
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			SetItemCursorWithInternalMargin(drawLeftTop);
			DrawAssetWriterPathSelectCombo();
			return GetItemRectWithInternalMargin();
		}
	);

	m_assetWriterPathComboBox.SetSelectableItems(m_assetMangerCached->GetAssetWriterPaths());

	m_assetWriterPathComboBox.OnSelChanged = [&](const size_t& idx, const string& selectedAssetName)
	{
		m_selectedAssetWriterPath = selectedAssetName;
	};
}

void AssetWriterPathNode::DrawAssetWriterPathSelectCombo()
{
	const float dynamicWidth = GetDynamicWidthWithoutInternalMargin(m_totalSize.x);

	PushItemWidth(dynamicWidth > defaultTextSize ? dynamicWidth : defaultTextSize);
	PushID(format("{}", (uint64_t)this).c_str());
	m_assetWriterPathComboBox.Draw();
	ConsumeMouseCaptureForItem();
	PopID();
	PopItemWidth();
}

std::string AssetWriterPathNode::GetVariableImpl(const std::tuple<>& variables)
{
	return m_selectedAssetWriterPath;
}
