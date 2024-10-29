#pragma once
#include "VariableOutputNode.h"

class MaterialAsset;
class BaseTextureAsset;
class AssetVariableInputPort;

class MaterialOutputNode : public VariableOutputNode
{
public:
	MaterialOutputNode(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		MaterialAsset* MaterialAssetIn
	);
	virtual ~MaterialOutputNode();

protected:
	MaterialAsset* MaterialAssetCached;

public:
	ConnectedHandler OnAmbientOcculusionTextureAssetSet;
	ConnectedHandler OnSpecularTextureAssetSet;
	ConnectedHandler OnDiffuseTextureAssetSet;
	ConnectedHandler OnRoughnessTextureAssetSet;
	ConnectedHandler OnMetalicTextureAssetSet;
	ConnectedHandler OnNormalTextureAssetSet;
	ConnectedHandler OnHeightTextureAssetSet;
	ConnectedHandler OnEmissiveTextureAssetSet;
	ConnectedHandler OnF0Set;

protected:
	const float PortToTextOffset = 10.f;
	const float PortRadius = 10.f;

protected:
	AssetVariableInputPort* AmbientOcculusionTexturePort = nullptr;
	AssetVariableInputPort* SpecularTexturePort = nullptr;
	AssetVariableInputPort* DiffuseTexturePort = nullptr;
	AssetVariableInputPort* RoughnessTexturePort = nullptr;
	AssetVariableInputPort* MetalicTexturePort = nullptr;
	AssetVariableInputPort* NormalTexturePort = nullptr;
	AssetVariableInputPort* HeightTexturePort = nullptr;
	AssetVariableInputPort* EmissiveTexturePort = nullptr;
	AssetVariableInputPort* F0Port = nullptr;
	MakeGetter(AmbientOcculusionTexturePort);
	MakeGetter(SpecularTexturePort);
	MakeGetter(DiffuseTexturePort);
	MakeGetter(RoughnessTexturePort);
	MakeGetter(MetalicTexturePort);
	MakeGetter(NormalTexturePort);
	MakeGetter(HeightTexturePort);
	MakeGetter(EmissiveTexturePort);
	MakeGetter(F0Port);

private:
	void SetAmbientOcculusionTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetSpecularTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetDiffuseTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetRoughnessTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetMetalicTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetNormalTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetHeightTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetEmissiveTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetF0WithConnect(AOutputPort* OutputPortIn);

private:
	std::shared_ptr<BaseTextureAsset> GetBaseTextureAssetFromOutputPort(AOutputPort* OutputPortIn);

public:
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;

private:
	void SetAssetDescription(
		const ImVec2& OriginPosition, 
		AssetVariableInputPort* AssetInputPortIn, 
		const std::string& AssetDescriptionIn
	);
};