#pragma once
#include "VariableInputPort.h"
#include "VariableOutputPort.h"
#include "Node.h"
#include "DrawElementColor.h"

#include <tuple>

template<typename OutputType, typename ...InputTypes>
class VariableNode : public Node
{
public:
	VariableNode(const std::string& nodeName, const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin);
	~VariableNode() override = default;
	
protected:
	std::tuple<VariableInputPort<InputTypes>...> m_variableInputPorts;
	VariableOutputPort<OutputType> m_variableOutputPort;

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);

protected:
	virtual ImVec2 GetInternalNodeSize() override;

private:
	template<std::size_t... Indices>
	std::tuple<VariableInputPort<InputTypes>...> CreateVariableInputPorts(
		Node* parentNode, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>);
};

template<typename OutputType, typename ...InputTypes>
inline VariableNode<OutputType, InputTypes...>::VariableNode(
	const std::string& nodeName, const ImVec2& leftTop,
	const float& radius, const ImVec2& referencedOrigin
)
	: Node(nodeName, leftTop, referencedOrigin, variableTypeColor),
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
inline ImVec2 VariableNode<OutputType, InputTypes...>::GetInternalNodeSize()
{
	float totalWidth = 0.f;
	float totalHeight = 0.f;

	std::apply([&](auto&... inputPorts) 
		{
			((totalWidth = std::max(inputPorts.GetTypeTextSize().x, totalWidth)), ...);
			((totalHeight += inputPorts.GetTypeTextSize().y), ...);
		}, m_variableInputPorts);

	return ImVec2(
		std::max({ totalWidth * 2.f, GetDrawNodeHeaderSize().x, nodeMinWidth }),
		std::max(totalHeight * 2.f, nodeMinHeight)
	);
}

template<typename OutputType, typename ...InputTypes>
template<std::size_t ...Indices>
inline std::tuple<VariableInputPort<InputTypes>...> VariableNode<OutputType, InputTypes...>::CreateVariableInputPorts(Node* parentNode, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>)
{
	return std::make_tuple(
		VariableInputPort<InputTypes>(parentNode, sizeof...(InputTypes), Indices, radius, referencedOrigin)...
	);
}
