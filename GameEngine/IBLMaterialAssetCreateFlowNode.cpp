#include "IBLMaterialAssetCreateFlowNode.h"

IBLMaterialAssetCreateFlowNode::IBLMaterialAssetCreateFlowNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: FlowNode("Create IBL Asset", leftTop, radius, referencedOrigin)
{
}

void IBLMaterialAssetCreateFlowNode::ExecuteImpl()
{
}
