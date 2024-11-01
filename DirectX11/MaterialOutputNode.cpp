#include "MaterialOutputNode.h"
#include "MaterialAsset.h"

#include "BaseTextureOutputPort.h"
#include "FloatVariableOutputPort.h"
#include "Float3VariableOutputPort.h"
#include "AssetVariableInputPort.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

MaterialOutputNode::MaterialOutputNode(
	const ImVec2& CenterPositionIn, 
	const ImVec2& RectangleSizeIn, 
	MaterialAsset* MaterialAssetIn
)
	: VariableOutputNode(CenterPositionIn, RectangleSizeIn), MaterialAssetCached(MaterialAssetIn)
{
	OnAmbientOcculusionTextureAssetSet = bind(&MaterialOutputNode::SetAmbientOcculusionTextureAssetWithConnect, this, placeholders::_1);
	OnSpecularTextureAssetSet = bind(&MaterialOutputNode::SetSpecularTextureAssetWithConnect, this, placeholders::_1);
	OnDiffuseTextureAssetSet = bind(&MaterialOutputNode::SetDiffuseTextureAssetWithConnect, this, placeholders::_1);
	OnRoughnessTextureAssetSet = bind(&MaterialOutputNode::SetRoughnessTextureAssetWithConnect, this, placeholders::_1);
	OnMetalicTextureAssetSet = bind(&MaterialOutputNode::SetMetalicTextureAssetWithConnect, this, placeholders::_1);
	OnNormalTextureAssetSet = bind(&MaterialOutputNode::SetNormalTextureAssetWithConnect, this, placeholders::_1);
	OnHeightTextureAssetSet = bind(&MaterialOutputNode::SetHeightTextureAssetWithConnect, this, placeholders::_1);
	OnEmissiveTextureAssetSet = bind(&MaterialOutputNode::SetEmissiveTextureAssetWithConnect, this, placeholders::_1);
	OnF0Set = bind(&MaterialOutputNode::SetF0WithConnect, this, placeholders::_1);
	OnHeightScaleSet = bind(&MaterialOutputNode::SetHeightSclaeWithConnect, this, placeholders::_1);

	AmbientOcculusionTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	AmbientOcculusionTexturePort->ConnectEvent += OnAmbientOcculusionTextureAssetSet;

	SpecularTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	SpecularTexturePort->ConnectEvent += OnSpecularTextureAssetSet;
	
	DiffuseTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	DiffuseTexturePort->ConnectEvent += OnDiffuseTextureAssetSet;
	
	RoughnessTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	RoughnessTexturePort->ConnectEvent += OnRoughnessTextureAssetSet;
	
	MetalicTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	MetalicTexturePort->ConnectEvent += OnMetalicTextureAssetSet;
	
	NormalTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	NormalTexturePort->ConnectEvent += OnNormalTextureAssetSet;
		
	EmissiveTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	EmissiveTexturePort->ConnectEvent += OnEmissiveTextureAssetSet;

	F0Port = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, Float3VariableOutputPort::Float3VariableKind);
	F0Port->ConnectEvent += OnF0Set;

	HeightTexturePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, BaseTextureAsset::BaseTextureAssetKind);
	HeightTexturePort->ConnectEvent += OnHeightTextureAssetSet;

	HeightScalePort = AddInputPort<AssetVariableInputPort>(ImVec2(NULL, NULL), PortRadius, FloatVariableOutputPort::FloatVariableKind);
	HeightScalePort->ConnectEvent += OnHeightScaleSet;
}

MaterialOutputNode::~MaterialOutputNode()
{
}

void MaterialOutputNode::SetAmbientOcculusionTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetAmbientOcculusionTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetSpecularTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetSpecularTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetDiffuseTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetDiffuseTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetRoughnessTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetRoughnessTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetMetalicTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetMetalicTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetNormalTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetNormalTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetHeightTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetHeightTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetEmissiveTextureAssetWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		MaterialAssetCached->SetEmissiveTextureAsset(GetBaseTextureAssetFromOutputPort(OutputPortIn));
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetF0WithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		Float3VariableOutputPort* Float3OutputIn = dynamic_cast<Float3VariableOutputPort*>(OutputPortIn);
		if (Float3OutputIn != nullptr)
		{
			MaterialAssetCached->SetF0(Float3OutputIn->GetFloat3Data());
		}
		else
		{
			MaterialAssetCached->SetF0(XMFLOAT3(0.f, 0.f, 0.f));
		}
		MaterialAssetCached->SetModified();
	}
}

void MaterialOutputNode::SetHeightSclaeWithConnect(AOutputPort* OutputPortIn)
{
	if (MaterialAssetCached)
	{
		FloatVariableOutputPort* FloatOutputIn = dynamic_cast<FloatVariableOutputPort*>(OutputPortIn);
		if (FloatOutputIn != nullptr)
		{
			MaterialAssetCached->SetHeightScale(FloatOutputIn->GetFloatData());
		}
		else
		{
			MaterialAssetCached->SetHeightScale(0.f);
		}
		MaterialAssetCached->SetModified();
	}
}

shared_ptr<BaseTextureAsset> MaterialOutputNode::GetBaseTextureAssetFromOutputPort(AOutputPort* OutputPortIn)
{
	BaseTextureOutputPort* BaseTextureOutputIn = dynamic_cast<BaseTextureOutputPort*>(OutputPortIn);
	if (BaseTextureOutputIn != nullptr)
	{
		return BaseTextureOutputIn->GetBaseTextureAssetCached();
	}
	return nullptr;
}

void MaterialOutputNode::AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn)
{
	NodeElement::AddToDrawList(OriginPosition, DrawListIn);
	SetAssetDescription(OriginPosition, AmbientOcculusionTexturePort, "AmbientOcculusion Texture");
	SetAssetDescription(OriginPosition, SpecularTexturePort, "Specular Texture");
	SetAssetDescription(OriginPosition, DiffuseTexturePort, "Diffuse Texture");
	SetAssetDescription(OriginPosition, RoughnessTexturePort, "Roughness Texture");
	SetAssetDescription(OriginPosition, MetalicTexturePort, "Metalic Texture");
	SetAssetDescription(OriginPosition, NormalTexturePort, "Normal Texture");
	SetAssetDescription(OriginPosition, EmissiveTexturePort, "Emissive Texture");
	SetAssetDescription(OriginPosition, F0Port, "F0 Constant");
	SetAssetDescription(OriginPosition, HeightTexturePort, "Height Texture");
	SetAssetDescription(OriginPosition, HeightScalePort, "Height Scale");
}

void MaterialOutputNode::SetAssetDescription(
	const ImVec2& OriginPosition, 
	AssetVariableInputPort* AssetInputPortIn, 
	const std::string& AssetDescriptionIn
)
{
	const ImVec2 AssetPortPosition = AssetInputPortIn->GetPosition();
	SetCursorScreenPos(ImVec2(
		AssetPortPosition.x + OriginPosition.x + PortRadius + PortToTextOffset, 
		AssetPortPosition.y + OriginPosition.y - PortRadius / 2.f
	));
	Text(AssetDescriptionIn.c_str());
}
