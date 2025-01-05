#pragma once
#include "APort.h"
#include "DrawElementColor.h"

template<typename ...InputTypes>
class FlowNode;

class FlowPort : public APort
{
public:
	template<typename ...InputTypes>
	FlowPort(
		FlowNode<InputTypes...>* parentNode, const bool& isLeft,
		size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~FlowPort() override = default;

protected:
	static FlowPort* m_connectingInputPort;
	static FlowPort* m_connectingOutputPort;

public:
	template<typename ...InputTypes>
	inline FlowNode<InputTypes...>* GetParentFlowNode() { return (FlowNode<InputTypes...>*)m_parentNode; }
};

template<typename ...InputTypes>
inline FlowPort::FlowPort(FlowNode<InputTypes...>* parentNode, const bool& isLeft, size_t indexCount, size_t portIndex, const float& radius, const ImVec2& referencedOrigin)
	: APort(parentNode, isLeft, indexCount, portIndex, radius, referencedOrigin, flowTypeColor, flowTypeHilightColor)
{
}