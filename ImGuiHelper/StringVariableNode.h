#pragma once
#include "VariableNode.h"

class StringVariableNode : public VariableNode<std::string>
{
public:
	StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~StringVariableNode() override = default;

protected:
	ImVec2 m_textInputSize = ImVec2(0.f, 0.f);
	std::string m_string;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;
	virtual ImVec2 GetInternalNodeSize() override;

protected:
	virtual std::string GetVariableImpl(const std::tuple<>& variables) override { return m_string; }
};

