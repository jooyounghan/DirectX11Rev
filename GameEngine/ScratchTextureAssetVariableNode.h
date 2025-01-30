#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

enum class EAssetType;

class AAsset;
class ScratchTextureAsset;

class ScratchTextureAssetVariableNode : public VariableNode<ScratchTextureAsset*>
{
public:
	ScratchTextureAssetVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~ScratchTextureAssetVariableNode() override = default;

protected:
	ScratchTextureAsset* m_selectedScratchTextureAsset = nullptr;

protected:
	ImGuiComboBox m_scratchTextureAssetComboBox;

private:
	void DrawImage();
	void DrawScratchAssetSelectCombo();
	void UpdateScratchAssetSelectables(const EAssetType&, std::string, AAsset*);

protected:
	virtual ScratchTextureAsset* GetVariableImpl(const std::tuple<>& variables) override;
};

