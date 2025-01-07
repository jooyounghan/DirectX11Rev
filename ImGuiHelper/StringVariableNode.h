#pragma once
#include "VariableNode.h"

class StringVariableNode : public VariableNode<std::string>
{
public:
	StringVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~StringVariableNode() override = default;

protected:
	std::string m_string;

private:
	void DrawStringEdit();

protected:
	virtual std::string GetVariableImpl(const std::tuple<>& variables) override { return m_string; }
};

