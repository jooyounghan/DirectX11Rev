#pragma once
#include "AAssetNodeCanvas.h"
#include "AddMaterialElementPopup.h"
#include "directxmath/DirectXMath.h"

class BaseTextureAsset;
class MaterialAsset;

class BaseTextureInputNode;
class Float3InputNode;
class FloatInputNode;

class AssetVariableInputPort;

class MaterialAssetNodeCanvas : public AAssetNodeCanvas
{
public:
	MaterialAssetNodeCanvas(AssetManager* AssetMangerIn, AAssetFile* AssetFileIn);
	virtual ~MaterialAssetNodeCanvas();

protected:
	MaterialAsset* MaterialAssetCached = nullptr;

protected:
	AddMaterialElementPopup AddMaterialElementPopupInstance;

protected:
	const float& NodeOffset = 25.f;
	const float& MaterialElementNodeWidth = 300.f;
	const float& MaterialElementNodeHeight = 100.f;
	const float& MaterialElementNodeXPos;
	float LastInsertedNodePosY;

protected:
	const float& MaterialNodeWidth = 200.f;
	const float& MaterialNodeHeight = 400.f;

public:
	virtual void RenderControl() override;

private:
	BaseTextureInputNode* AddBaseTextureInputNodeHelper(float& NodeHeightPos, const std::shared_ptr<BaseTextureAsset>& BaseTextureAssetIn);
	BaseTextureInputNode* AddBaseTextureInputNodeHelper(const ImVec2& NodePos, const std::shared_ptr<BaseTextureAsset>& BaseTextureAssetIn);
	void ConnectBaseTextureToMaterial(BaseTextureInputNode* BaseTextureInputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn);
	
private:
	Float3InputNode* AddF0InputNodeHelper(float& NodeHeightPos, const DirectX::XMFLOAT3& F0In);
	Float3InputNode* AddF0InputNodeHelper(const ImVec2& NodePos, const DirectX::XMFLOAT3& F0In);
	void ConnectF0ToMaterial(Float3InputNode* F0InputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn);

private:
	FloatInputNode* AddHeightScaleInputNodeHelper(float& NodeHeightPos, const float& HeightIn);
	FloatInputNode* AddHeightScaleInputNodeHelper(const ImVec2& NodePos, const float& HeightIn);
	void ConnectHeightScaleToMaterial(FloatInputNode* HeightScaleInputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn);

private:
	BaseTextureAddedHandler OnBaseTextureAdded;
	void AddBaseTextureNode();

private:
	Float3AddedHandler OnFloat3Added;
	void AddF0ConstantNode();

private:
	FloatAddedHandler OnFloatAdded;
	void AddHeightScaleConstantNode();
};

