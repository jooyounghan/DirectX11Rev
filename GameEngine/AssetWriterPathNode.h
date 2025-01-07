#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

class AssetManager;

class AssetWriterPathNode : public VariableNode<std::string>
{
public:
	AssetWriterPathNode(
		const ImVec2& leftTop, const float& radius, 
		const ImVec2& referencedOrigin, AssetManager* assetManager
	);
	~AssetWriterPathNode() override = default;

protected:
	AssetManager* m_assetMangerCached = nullptr;
	std::string m_selectedAssetWriterPath;

protected:
	ImGuiComboBox m_assetWriterPathComboBox;

private:
	void DrawAssetWriterPathSelectCombo();

protected:
	virtual std::string GetVariableImpl(const std::tuple<>& variables) override;

};

