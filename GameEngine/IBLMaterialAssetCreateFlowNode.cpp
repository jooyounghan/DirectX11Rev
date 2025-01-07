#include "IBLMaterialAssetCreateFlowNode.h"
#include "AssetWrapper.h"
#include "IBLMaterialAsset.h"

using namespace std;

IBLMaterialAssetCreateFlowNode::IBLMaterialAssetCreateFlowNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: FlowNode(
		"Create IBL Asset", leftTop, radius, referencedOrigin, 
		{"Asset Path", "File Name", "Background", "BRDR", "Specular Cube Map", "Diffuse Cube Map", "Exposure", "Gamma"})
{
	AddDrawCommand([&](const ImVec2& drawLeftTop, ImDrawList* drawListIn)
		{
			return GetMinNodeSize();
		});
}

void IBLMaterialAssetCreateFlowNode::ExecuteImpl()
{
	auto inputVariables = GetInputVariables();
	const string& assetPath = get<0>(inputVariables);
	const string& assetName = get<1>(inputVariables);
	ScratchTextureAsset* background = get<2>(inputVariables);
	ScratchTextureAsset* brdf = get<3>(inputVariables);
	ScratchTextureAsset* specular = get<4>(inputVariables);
	ScratchTextureAsset* diffuse = get<5>(inputVariables);
	const float& exposure = get<6>(inputVariables);
	const float& gamma = get<7>(inputVariables);

	bool test1 = true;
}
