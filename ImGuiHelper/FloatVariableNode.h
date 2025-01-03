#pragma once
#include "VariableNode.h"
#include "FundamentalVariableOutputPort.h"

class FloatVariableNode : public VariableNode<float, FundamentalVariableOutputPort<float>>
{
public:
	FloatVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~FloatVariableNode() override = default;

protected:
	ImVec2 m_dragFloatItemSize = ImVec2(0.f, 0.f);

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual ImVec2 GetInternalNodeSize() override;
};

