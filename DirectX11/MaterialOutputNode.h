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
	ConnectedDelegation OnAmbientOcculusionTextureAssetSet;
	ConnectedDelegation OnSpecularTextureAssetSet;
	ConnectedDelegation OnDiffuseTextureAssetSet;
	ConnectedDelegation OnRoughnessTextureAssetSet;
	ConnectedDelegation OnMetalicTextureAssetSet;
	ConnectedDelegation OnNormalTextureAssetSet;
	ConnectedDelegation OnHeightTextureAssetSet;
	ConnectedDelegation OnEmissiveTextureAssetSet;

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
	MakeGetter(AmbientOcculusionTexturePort);
	MakeGetter(SpecularTexturePort);
	MakeGetter(DiffuseTexturePort);
	MakeGetter(RoughnessTexturePort);
	MakeGetter(MetalicTexturePort);
	MakeGetter(NormalTexturePort);
	MakeGetter(HeightTexturePort);
	MakeGetter(EmissiveTexturePort);

private:
	void SetAmbientOcculusionTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetSpecularTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetDiffuseTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetRoughnessTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetMetalicTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetNormalTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetHeightTextureAssetWithConnect(AOutputPort* OutputPortIn);
	void SetEmissiveTextureAssetWithConnect(AOutputPort* OutputPortIn);

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