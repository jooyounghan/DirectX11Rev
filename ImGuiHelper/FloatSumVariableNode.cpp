#include "FloatSumVariableNode.h"
#include <array>

FloatSumVariableNode::FloatSumVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: VariableNode("Sum Float", leftTop, radius, referencedOrigin, {"Float, Float"})
{
}

float FloatSumVariableNode::GetVariableImpl(const std::tuple<float, float>& variables)
{
    return get<0>(variables) + get<1>(variables);
}
