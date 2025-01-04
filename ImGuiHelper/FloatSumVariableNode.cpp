#include "FloatSumVariableNode.h"

float FloatSumVariableNode::GetVariableImpl(const std::tuple<float, float>& variables)
{
    return get<0>(variables) + get<1>(variables);
}
