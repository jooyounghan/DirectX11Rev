#include "MaterialAssetNodeCanvas.h"
#include "MaterialAsset.h"

#include "BaseTextureInputNode.h"
#include "FloatInputNode.h"
#include "Float3InputNode.h"
#include "MaterialOutputNode.h"

#include "AssetVariableInputPort.h"
#include "BaseTextureOutputPort.h"
#include "Float3VariableOutputPort.h"
#include "FloatVariableOutputPort.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;

MaterialAssetNodeCanvas::MaterialAssetNodeCanvas(AAssetFile* AssetFileIn)
    : AAssetNodeCanvas(AssetFileIn, ImVec2(700.f, 1200.f)), MaterialAssetCached(dynamic_cast<MaterialAsset*>(AssetFileIn)),
    MaterialElementNodeXPos(MaterialElementNodeWidth / 2.f + NodeOffset),
    AddMaterialElementPopupInstance(format("AddBaseTextureForMaterial{}", (uint64_t)this))
{
    float NodeHeightInitialPosY = MaterialElementNodeHeight / 2.f + NodeOffset;
    LastInsertedNodePosY = NodeHeightInitialPosY;

    const shared_ptr<BaseTextureAsset>& AmbientOcculusion = MaterialAssetCached->GetAmbientOcculusionTextureAsset();
    const shared_ptr<BaseTextureAsset>& Specular = MaterialAssetCached->GetSpecularTextureAsset();
    const shared_ptr<BaseTextureAsset>& Diffuse = MaterialAssetCached->GetDiffuseTextureAsset();
    const shared_ptr<BaseTextureAsset>& Roughness = MaterialAssetCached->GetRoughnessTextureAsset();
    const shared_ptr<BaseTextureAsset>& Metalic = MaterialAssetCached->GetMetalicTextureAsset();
    const shared_ptr<BaseTextureAsset>& Normal = MaterialAssetCached->GetNormalTextureAsset();
    const shared_ptr<BaseTextureAsset>& Height = MaterialAssetCached->GetHeightTextureAsset();
    const shared_ptr<BaseTextureAsset>& Emissive = MaterialAssetCached->GetEmissiveTextureAsset();
    const XMFLOAT3& F0 = MaterialAssetCached->GetF0();
    const float& HeightScale = MaterialAssetCached->GetHeightScale();

    BaseTextureInputNode* AmbientOcculusionInputNode = AmbientOcculusion != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, AmbientOcculusion) : nullptr;
    BaseTextureInputNode* SpecularInputNode = Specular != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Specular) : nullptr;
    BaseTextureInputNode* DiffuseInputNode = Diffuse != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Diffuse) : nullptr;
    BaseTextureInputNode* RoughnessInputNode = Roughness != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Roughness) : nullptr;
    BaseTextureInputNode* MetalicInputNode = Metalic != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Metalic) : nullptr;
    BaseTextureInputNode* NormalInputNode = Normal != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Normal) : nullptr;
    BaseTextureInputNode* EmissiveInputNode = Emissive != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Emissive) : nullptr;
    Float3InputNode* F0InputNode = AddF0InputNodeHelper(LastInsertedNodePosY, F0);
    BaseTextureInputNode* HeightInputNode = Height != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Height) : nullptr;
    FloatInputNode* HeightScaleInputNode = AddHeightScaleInputNodeHelper(LastInsertedNodePosY, HeightScale);

    const float MaterialNodePosX = MaterialElementNodeXPos + MaterialElementNodeWidth + NodeOffset * 2.f;
    const float MaterialNodePosY = NodeHeightInitialPosY + (LastInsertedNodePosY - (MaterialElementNodeHeight + NodeOffset) - NodeHeightInitialPosY) / 2.f;

    MaterialOutputNode* MaterialAssetOutputNode = AddNodeElement<MaterialOutputNode>(
        ImVec2(MaterialNodePosX, MaterialNodePosY), 
        ImVec2(MaterialNodeWidth, MaterialNodeHeight), 
        MaterialAssetCached
    );

    ConnectBaseTextureToMaterial(AmbientOcculusionInputNode, MaterialAssetOutputNode->GetAmbientOcculusionTexturePort());
    ConnectBaseTextureToMaterial(SpecularInputNode, MaterialAssetOutputNode->GetSpecularTexturePort());
    ConnectBaseTextureToMaterial(DiffuseInputNode, MaterialAssetOutputNode->GetDiffuseTexturePort());
    ConnectBaseTextureToMaterial(RoughnessInputNode, MaterialAssetOutputNode->GetRoughnessTexturePort());
    ConnectBaseTextureToMaterial(MetalicInputNode, MaterialAssetOutputNode->GetMetalicTexturePort());
    ConnectBaseTextureToMaterial(NormalInputNode, MaterialAssetOutputNode->GetNormalTexturePort());
    ConnectBaseTextureToMaterial(HeightInputNode, MaterialAssetOutputNode->GetHeightTexturePort());
    ConnectBaseTextureToMaterial(EmissiveInputNode, MaterialAssetOutputNode->GetEmissiveTexturePort());
    ConnectF0ToMaterial(F0InputNode, MaterialAssetOutputNode->GetF0Port());
    ConnectHeightScaleToMaterial(HeightScaleInputNode, MaterialAssetOutputNode->GetHeightScalePort());

    OnBaseTextureAdded = bind(&MaterialAssetNodeCanvas::AddBaseTextureNode, this);
    OnFloat3Added = bind(&MaterialAssetNodeCanvas::AddF0ConstantNode, this);
    OnFloatAdded = bind(&MaterialAssetNodeCanvas::AddHeightScaleConstantNode, this);

    AddMaterialElementPopupInstance.BaseTextureAdded += OnBaseTextureAdded;
    AddMaterialElementPopupInstance.Float3Added += OnFloat3Added;
    AddMaterialElementPopupInstance.FloatAdded += OnFloatAdded;
}

MaterialAssetNodeCanvas::~MaterialAssetNodeCanvas()
{
}

void MaterialAssetNodeCanvas::RenderControl()
{
    AAssetNodeCanvas::RenderControl();
    AddMaterialElementPopupInstance.PopUp(LeftTopPositon, RightBottomPosition);
}

BaseTextureInputNode* MaterialAssetNodeCanvas::AddBaseTextureInputNodeHelper(float& NodeHeightPos, const shared_ptr<BaseTextureAsset>& BaseTextureAssetIn)
{
    BaseTextureInputNode* AddedInputNode = AddNodeElement<BaseTextureInputNode>(ImVec2(MaterialElementNodeXPos, NodeHeightPos), ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), BaseTextureAssetIn);
    NodeHeightPos += (MaterialElementNodeHeight + NodeOffset);
    return AddedInputNode;
}

BaseTextureInputNode* MaterialAssetNodeCanvas::AddBaseTextureInputNodeHelper(const ImVec2& NodePos, const shared_ptr<BaseTextureAsset>& BaseTextureAssetIn)
{
    BaseTextureInputNode* AddedInputNode = AddNodeElement<BaseTextureInputNode>(NodePos, ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), BaseTextureAssetIn);
    return AddedInputNode;
}

void MaterialAssetNodeCanvas::ConnectBaseTextureToMaterial(BaseTextureInputNode* BaseTextureInputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn)
{
    if (BaseTextureInputNodeIn != nullptr && MaterialTextureInputNodeIn != nullptr)
    {
        BaseTextureOutputPort* BaseTextureOutputPortCached = BaseTextureInputNodeIn->GetBaseTextureOutputPortCached();

        if (BaseTextureOutputPortCached != nullptr)
        {
            BaseTextureOutputPortCached->Connect(MaterialTextureInputNodeIn);
        }
    }
}

Float3InputNode* MaterialAssetNodeCanvas::AddF0InputNodeHelper(float& NodeHeightPos, const XMFLOAT3& F0In)
{
    Float3InputNode* AddedInputNode = AddNodeElement<Float3InputNode>(ImVec2(MaterialElementNodeXPos, NodeHeightPos), ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), F0In);
    NodeHeightPos += (MaterialElementNodeHeight + NodeOffset);
    return AddedInputNode;
}

Float3InputNode* MaterialAssetNodeCanvas::AddF0InputNodeHelper(const ImVec2& NodePos, const XMFLOAT3& F0In)
{
    Float3InputNode* AddedInputNode = AddNodeElement<Float3InputNode>(NodePos, ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), F0In);
    return AddedInputNode;
}

void MaterialAssetNodeCanvas::ConnectF0ToMaterial(Float3InputNode* F0InputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn)
{
    if (F0InputNodeIn != nullptr && MaterialTextureInputNodeIn != nullptr)
    {
        Float3VariableOutputPort* Float3OutputPortCached = F0InputNodeIn->GetFloat3VariableOutputPortCached();

        if (Float3OutputPortCached != nullptr)
        {
            Float3OutputPortCached->Connect(MaterialTextureInputNodeIn);
        }
    }
}

FloatInputNode* MaterialAssetNodeCanvas::AddHeightScaleInputNodeHelper(float& NodeHeightPos, const float& HeightIn)
{
    FloatInputNode* AddedInputNode = AddNodeElement<FloatInputNode>(ImVec2(MaterialElementNodeXPos, NodeHeightPos), ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), HeightIn);
    NodeHeightPos += (MaterialElementNodeHeight + NodeOffset);
    return AddedInputNode;
}

FloatInputNode* MaterialAssetNodeCanvas::AddHeightScaleInputNodeHelper(const ImVec2& NodePos, const float& HeightIn)
{
    FloatInputNode* AddedInputNode = AddNodeElement<FloatInputNode>(NodePos, ImVec2(MaterialElementNodeWidth, MaterialElementNodeHeight), HeightIn);
    return AddedInputNode;
}

void MaterialAssetNodeCanvas::ConnectHeightScaleToMaterial(FloatInputNode* HeightScaleInputNodeIn, AssetVariableInputPort* MaterialTextureInputNodeIn)
{
    if (HeightScaleInputNodeIn != nullptr && MaterialTextureInputNodeIn != nullptr)
    {
        FloatVariableOutputPort* Float3OutputPortCached = HeightScaleInputNodeIn->GetFloatVariableOutputPortCached();

        if (Float3OutputPortCached != nullptr)
        {
            Float3OutputPortCached->Connect(MaterialTextureInputNodeIn);
        }
    }
}

void MaterialAssetNodeCanvas::AddBaseTextureNode()
{
    const ImVec2 MousePos = GetMousePos();
    const ImVec2 CanvasPos = ImVec2(MousePos.x - OriginPosition.x, MousePos.y - OriginPosition.y);
    AddBaseTextureInputNodeHelper(CanvasPos, nullptr);

}

void MaterialAssetNodeCanvas::AddF0ConstantNode()
{
    const ImVec2 MousePos = GetMousePos();
    const ImVec2 CanvasPos = ImVec2(MousePos.x - OriginPosition.x, MousePos.y - OriginPosition.y);
    AddF0InputNodeHelper(CanvasPos, XMFLOAT3(0.f, 0.f, 0.f));
}

void MaterialAssetNodeCanvas::AddHeightScaleConstantNode()
{
    const ImVec2 MousePos = GetMousePos();
    const ImVec2 CanvasPos = ImVec2(MousePos.x - OriginPosition.x, MousePos.y - OriginPosition.y);
    AddHeightScaleInputNodeHelper(CanvasPos, 0.f);
}
