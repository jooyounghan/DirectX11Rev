#include "MaterialAssetNodeCanvas.h"
#include "MaterialAsset.h"

#include "BaseTextureInputNode.h"
#include "MaterialOutputNode.h"

#include "AssetVariableInputPort.h"
#include "BaseTextureOutputPort.h"

using namespace std;
using namespace ImGui;


MaterialAssetNodeCanvas::MaterialAssetNodeCanvas(AssetManager* AssetMangerIn, AAssetFile* AssetFileIn)
    : AAssetNodeCanvas(AssetMangerIn, AssetFileIn, ImVec2(700.f, 1200.f)), MaterialAssetCached(dynamic_cast<MaterialAsset*>(AssetFileIn)),
    BaseTextureNodeXPos(BaseTextureNodeWidth / 2.f + NodeOffset)
{
    float NodeHeightInitialPosY = BaseTextureNodeHeight / 2.f + NodeOffset;
    LastInsertedNodePosY = NodeHeightInitialPosY;

    const shared_ptr<BaseTextureAsset>& AmbientOcculusion = MaterialAssetCached->GetAmbientOcculusionTextureAsset();
    const shared_ptr<BaseTextureAsset>& Specular = MaterialAssetCached->GetSpecularTextureAsset();
    const shared_ptr<BaseTextureAsset>& Diffuse = MaterialAssetCached->GetDiffuseTextureAsset();
    const shared_ptr<BaseTextureAsset>& Roughness = MaterialAssetCached->GetRoughnessTextureAsset();
    const shared_ptr<BaseTextureAsset>& Metalic = MaterialAssetCached->GetMetalicTextureAsset();
    const shared_ptr<BaseTextureAsset>& Normal = MaterialAssetCached->GetNormalTextureAsset();
    const shared_ptr<BaseTextureAsset>& Height = MaterialAssetCached->GetHeightTextureAsset();
    const shared_ptr<BaseTextureAsset>& Emissive = MaterialAssetCached->GetEmissiveTextureAsset();

    BaseTextureInputNode* AmbientOcculusionInputNode = AmbientOcculusion != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, AmbientOcculusion) : nullptr;
    BaseTextureInputNode* SpecularInputNode = Specular != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Specular) : nullptr;
    BaseTextureInputNode* DiffuseInputNode = Diffuse != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Diffuse) : nullptr;
    BaseTextureInputNode* RoughnessInputNode = Roughness != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Roughness) : nullptr;
    BaseTextureInputNode* MetalicInputNode = Metalic != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Metalic) : nullptr;
    BaseTextureInputNode* NormalInputNode = Normal != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Normal) : nullptr;
    BaseTextureInputNode* HeightInputNode = Height != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Height) : nullptr;
    BaseTextureInputNode* EmissiveInputNode = Emissive != nullptr ? AddBaseTextureInputNodeHelper(LastInsertedNodePosY, Emissive) : nullptr;

    const float MaterialNodePosX = BaseTextureNodeXPos + BaseTextureNodeWidth + NodeOffset * 2.f;
    const float MaterialNodePosY = NodeHeightInitialPosY + (LastInsertedNodePosY - (BaseTextureNodeHeight + NodeOffset) - NodeHeightInitialPosY) / 2.f;

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
}

MaterialAssetNodeCanvas::~MaterialAssetNodeCanvas()
{
}

void MaterialAssetNodeCanvas::ShowContextMenu(const ImVec2& OriginPosition)
{
    ANodeCanvas::ShowContextMenu(OriginPosition);
    if (BeginPopup(ContextPopUpID))
    {
        if (MenuItem("Add BaseTexture Asset", NULL, false, true)) 
        { 
            const ImVec2 MousePos = GetMousePos();
            const ImVec2 CanvasPos = ImVec2(MousePos.x - OriginPosition.x, MousePos.y - OriginPosition.y);
            AddBaseTextureInputNodeHelper(CanvasPos, nullptr);
        }
        EndPopup();
    }
}

BaseTextureInputNode* MaterialAssetNodeCanvas::AddBaseTextureInputNodeHelper(float& NodeHeightPos, const shared_ptr<BaseTextureAsset>& BaseTextureAssetIn)
{
    BaseTextureInputNode* AddedInputNode = AddNodeElement<BaseTextureInputNode>(ImVec2(BaseTextureNodeXPos, NodeHeightPos), ImVec2(BaseTextureNodeWidth, BaseTextureNodeHeight), AssetManagerCached, BaseTextureAssetIn);
    NodeHeightPos += (BaseTextureNodeHeight + NodeOffset);
    return AddedInputNode;
}

BaseTextureInputNode* MaterialAssetNodeCanvas::AddBaseTextureInputNodeHelper(const ImVec2& NodePos, const shared_ptr<BaseTextureAsset>& BaseTextureAssetIn)
{
    BaseTextureInputNode* AddedInputNode = AddNodeElement<BaseTextureInputNode>(NodePos, ImVec2(BaseTextureNodeWidth, BaseTextureNodeHeight), AssetManagerCached, BaseTextureAssetIn);
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
