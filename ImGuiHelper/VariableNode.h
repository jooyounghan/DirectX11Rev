#pragma once
#include "VariableInputPort.h"
#include "VariableOutputPort.h"
#include "Node.h"

#include <tuple>

template<typename OutputType, typename ...InputTypes>
class VariableNode : public Node
{
public:
	VariableNode(const std::string& nodeName, const ImVec2& leftTop, const ImVec2& size, const float& radius, const ImVec2& referencedOrigin);
	~VariableNode() override = default;
	
protected:
	std::tuple<VariableInputPort<InputTypes>...> m_variableInputPorts;
	VariableOutputPort<OutputType> m_variableOutputPort;

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);

private:
	template<std::size_t... Indices>
	std::tuple<VariableInputPort<InputTypes>...> CreateVariableInputPorts(
		Node* parentNode, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>);
};

template<typename OutputType, typename ...InputTypes>
inline VariableNode<OutputType, InputTypes...>::VariableNode(
	const std::string& nodeName, const ImVec2& leftTop, const ImVec2& size,
	const float& radius, const ImVec2& referencedOrigin
)
	: Node(
		nodeName, leftTop, size, referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
	),
	m_variableInputPorts(CreateVariableInputPorts(this, radius, referencedOrigin, std::index_sequence_for<InputTypes...>{})),
	m_variableOutputPort(this, 1, 0, radius, referencedOrigin)
{
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::AddToDrawElementManager(drawElementManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.AddToDrawElementManager(drawElementManager)); }, m_variableInputPorts);
	m_variableOutputPort.AddToDrawElementManager(drawElementManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::RemoveFromDrawElementManager(drawElementManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.RemoveFromDrawElementManager(drawElementManager)); }, m_variableInputPorts);
	m_variableOutputPort.RemoveFromDrawElementManager(drawElementManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::RegisterToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.RegisterToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_variableOutputPort.RegisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::DeregisterToInteractionManager(InteractionManager* interactionManager)
{	
	Node::DeregisterToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.DeregisterToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_variableOutputPort.DeregisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
template<std::size_t ...Indices>
inline std::tuple<VariableInputPort<InputTypes>...> VariableNode<OutputType, InputTypes...>::CreateVariableInputPorts(Node* parentNode, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>)
{
	return std::make_tuple(
		VariableInputPort<InputTypes>(parentNode, sizeof...(InputTypes), Indices, radius, referencedOrigin)...
	);
}
