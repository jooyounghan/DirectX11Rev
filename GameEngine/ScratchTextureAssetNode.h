#pragma once
#include "VariableNode.h"

class AssetManager;
class ScratchTextureAsset;

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
	ImVec2 m_imageSize = ImVec2(0.f, 0.f);
	ImVec2 m_comboSize = ImVec2(0.f, 0.f);

protected:
	virtual ScratchTextureAsset* GetVariableImpl(const std::tuple<>& variables) override;
};

