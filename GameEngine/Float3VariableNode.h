#pragma once
#include "VariableNode.h"
#include <DirectXMath.h>

class Float3VariableNode : public VariableNode<DirectX::XMFLOAT3>
{
public:
	Float3VariableNode(const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~Float3VariableNode() override = default;

protected:
	DirectX::XMFLOAT3 m_float3 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

private:
	void DrawDragFloat3();

protected:
	virtual DirectX::XMFLOAT3 GetVariableImpl(const std::tuple<>& variables) override { return m_float3; }
};

