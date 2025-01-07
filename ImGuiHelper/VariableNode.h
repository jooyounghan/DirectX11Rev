#pragma once
#include "Node.h"
#include "VariableInputPort.h"
#include "VariableOutputPort.h"
#include "DrawElementColor.h"

#include <tuple>
#include <exception>
#include <array>

template<typename OutputType, typename ...InputTypes>
class VariableNode : public Node
{
public:
	VariableNode(
		const std::string& nodeName, const ImVec2& leftTop, const float& radius, 
		const ImVec2& referencedOrigin, const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions);
	~VariableNode() override = default;
	
protected:
	std::tuple<VariableInputPort<InputTypes>...> m_variableInputPorts;
	VariableOutputPort<OutputType> m_variableOutputPort;

public:
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);
	virtual void BringFrontToInteractionManager(InteractionManager* interactionManager);

protected:
	virtual void SetFocused(const bool& isFocused);protected:
	virtual void Draw(ImDrawList* drawListIn) override final;

public:
	virtual OutputType GetVariable();

protected:
	virtual OutputType GetVariableImpl(const std::tuple<InputTypes...>& variables) = 0;
};

template<typename... InputTypes, std::size_t... Indices>
std::tuple<VariableInputPort<InputTypes>...> CreateVariableInputPorts(
	Node* parentNode, const float& radius, const ImVec2& referencedOrigin, const size_t& offset, 
	const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions, std::index_sequence<Indices...>
);

template<typename... InputTypes, std::size_t... Indices>
std::tuple<InputTypes...> GetVariablesFromInput(const std::tuple<VariableInputPort<InputTypes>...>& inputPorts, std::index_sequence<Indices...>);


template<typename OutputType, typename ...InputTypes>
inline VariableNode<OutputType, InputTypes...>::VariableNode(
	const std::string& nodeName, const ImVec2& leftTop, const float& radius, 
	const ImVec2& referencedOrigin, const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions
)
	: Node(nodeName, leftTop, referencedOrigin, variableTypeColor),
	m_variableInputPorts(CreateVariableInputPorts<InputTypes...>(this, radius, referencedOrigin, 0, inputDescriptions, std::index_sequence_for<InputTypes...>{})),
	m_variableOutputPort(this, 1, 0, radius, referencedOrigin)
{

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
inline void VariableNode<OutputType, InputTypes...>::BringFrontToInteractionManager(InteractionManager* interactionManager)
{
	Node::BringFrontToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.BringFrontToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_variableOutputPort.BringFrontToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::SetFocused(const bool& isFocused)
{
	Node::SetFocused(isFocused);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.SetFocused(isFocused)); }, m_variableInputPorts);
	m_variableOutputPort.SetFocused(isFocused);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::Draw(ImDrawList* drawListIn)
{
	Node::Draw(drawListIn);
	std::apply([&](auto&... inputPorts){ (..., inputPorts.Draw(drawListIn)); }, m_variableInputPorts);
	m_variableOutputPort.Draw(drawListIn);
}


template<typename OutputType, typename ...InputTypes>
inline OutputType VariableNode<OutputType, InputTypes...>::GetVariable()
{
	return GetVariableImpl(GetVariablesFromInput(m_variableInputPorts, std::index_sequence_for<InputTypes...>{}));
}

template<typename ...InputTypes, std::size_t ...Indices>
inline std::tuple<VariableInputPort<InputTypes>...> CreateVariableInputPorts(
	Node* parentNode, const float& radius, const ImVec2& referencedOrigin, const size_t& offset, 
	const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions, std::index_sequence<Indices...>
)
{
	return std::make_tuple(
		VariableInputPort<InputTypes>(parentNode, sizeof...(InputTypes) + offset, Indices + offset, radius, referencedOrigin, inputDescriptions.at(Indices))...
	);
}

template<typename ...InputTypes, std::size_t ...Indices>
inline std::tuple<InputTypes...> GetVariablesFromInput(const std::tuple<VariableInputPort<InputTypes>...>& inputPorts, std::index_sequence<Indices...>)
{
	return std::make_tuple(
		(
			[&]() -> InputTypes 
			{
				auto* connectedOutputPort = std::get<Indices>(inputPorts).GetConnectedOutputPort();
				return connectedOutputPort
					? connectedOutputPort->GetParentVariableNode()->GetVariable()
					: throw std::exception("Variable Input are Not Connected");
			}()
		)...
	);
}
