#pragma once
#include "VariableNode.h"

class FloatSumVariableNode : public VariableNode<float, float, float>
{
public:
	FloatSumVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
		: VariableNode("Sum Float", leftTop, radius, referencedOrigin) {
	}
	~FloatSumVariableNode() override = default;

protected:
	virtual float GetVariableImpl(const std::tuple<float, float>& variables) override;
};

