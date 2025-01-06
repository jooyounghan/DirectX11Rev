#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

enum class EAssetType;

class AAsset;
class AssetManager;
class ScratchTextureAsset;
class ImGuiComboBox;

class ScratchTextureAssetNode : public VariableNode<ScratchTextureAsset*>
{
public:
	ScratchTextureAssetNode(
		const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin, AssetManager* assetManager
	);
	~ScratchTextureAssetNode() override = default;

protected:
	AssetManager* m_assetMangerCached = nullptr;
	ScratchTextureAsset* m_selectedScratchTextureAsset = nullptr;

protected:
	ImGuiComboBox m_scratchTextureAssetComboBox;

private:
	void DrawDrawImage();
	void DrawScratchAssetSelectCombo();
	void UpdateScratchAssetSelectables(const EAssetType&, std::string, AAsset*);

protected:
	virtual ScratchTextureAsset* GetVariableImpl(const std::tuple<>& variables) override;
};

