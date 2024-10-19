#pragma once
#include "AAssetNodeCanvas.h"

class BaseTextureAsset;
class MaterialAsset;

class BaseTextureInputNode;
class AssetVariableInputPort;

class MaterialAssetNodeCanvas : public AAssetNodeCanvas
{
public:
	MaterialAssetNodeCanvas(AssetManager* AssetMangerIn, AAssetFile* AssetFileIn);
	virtual ~MaterialAssetNodeCanvas();

protected:
	MaterialAsset* MaterialAssetCached = nullptr;

protected:
	const float& NodeOffset = 25.f;
	const float& BaseTextureNodeWidth = 300.f;
	const float& BaseTextureNodeHeight = 100.f;
	const float& BaseTextureNodeXPos;
	float LastInsertedNodePosY;

protected:
	const float& MaterialNodeWidth = 200.f;
	const float& MaterialNodeHeight = 400.f;

protected:
	virtual void ShowContextMenu(const ImVec2& OriginPosition) override;

private:
	BaseTextureInputNode* AddBaseTextureInputNodeHelper(float& NodeHeightPos, const std::shared_ptr<BaseTextureAsset>& BaseTextureAssetIn);
	BaseTextureInputNode* AddBaseTextureInputNodeHelper(const ImVec2& NodePos, const std::shared_ptr<BaseTextureAsset>& BaseTextureAssetIn);
	void ConnectBaseTextureToMaterial(BaseTextureInputNode* BaseTextureInputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn);
};

