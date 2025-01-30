#pragma once
#include "VariableNode.h"
#include "ImGuiComboBox.h"

enum class EAssetType;

class AAsset;
class BaseTextureAsset;

class BaseTextureAssetVariableNode : public VariableNode<BaseTextureAsset*>
{
public:
	BaseTextureAssetVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~BaseTextureAssetVariableNode() override = default;

protected:
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

