#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

enum class EAssetType;

class AAsset;
class AssetManager;
class BaseTextureAsset;

class BaseTextureAssetVariableNode : public VariableNode<BaseTextureAsset*>
{
public:
	BaseTextureAssetVariableNode(
		const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin, AssetManager* assetManager
	);
	~BaseTextureAssetVariableNode() override = default;

protected:
	AssetManager* m_assetMangerCached = nullptr;
	BaseTextureAsset* m_selectedBaseTextureAsset = nullptr;

protected:
	ImGuiComboBox m_baseTextureAssetComboBox;

private:
	void DrawImage();
	void DrawBaseAssetSelectCombo();
	void UpdateBaseAssetSelectables(const EAssetType&, std::string, AAsset*);

protected:
	virtual BaseTextureAsset* GetVariableImpl(const std::tuple<>& variables) override;

};

