#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

class AssetWriterPathNode : public VariableNode<std::string>
{
public:
	AssetWriterPathNode(
		const ImVec2& leftTop, const float& radius, 
		const ImVec2& referencedOrigin
	);
	~AssetWriterPathNode() override = default;

protected:
	std::string m_selectedAssetWriterPath;

protected:
	ImGuiComboBox m_assetWriterPathComboBox;

private:
	void DrawAssetWriterPathSelectCombo();

protected:
	virtual std::string GetVariableImpl(const std::tuple<>& variables) override;

};

