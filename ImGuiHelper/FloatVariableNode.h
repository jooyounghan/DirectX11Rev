#pragma once
#include "VariableNode.h"

class FloatVariableNode : public VariableNode<float>
{
public:
	FloatVariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~FloatVariableNode() override = default;

protected:
	float m_float = 0.f;

private:
	void DrawDragFloat();

protected:
	virtual float GetVariableImpl(const std::tuple<>& variables) override { return m_float; }
};

