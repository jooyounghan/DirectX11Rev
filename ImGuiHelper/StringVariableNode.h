#pragma once
#include "VariableNode.h"
#include "StringVariableOutputPort.h"

class StringVariableNode : public VariableNode<std::string, StringVariableOutputPort>
{
public:
	StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~StringVariableNode() override = default;

protected:
	ImVec2 m_textInputSize = ImVec2(0.f, 0.f);

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual ImVec2 GetInternalNodeSize() override;
};

