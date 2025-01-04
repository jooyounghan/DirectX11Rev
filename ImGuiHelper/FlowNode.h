#pragma once
#include "VariableNode.h"
#include "FlowInputPort.h"
#include "FlowOutputPort.h"


template<typename ...InputTypes>
class FlowNode : public Node
{
public:
	FlowNode(
		const std::string& nodeName, const ImVec2& leftTop,
		const float& radius, const ImVec2& referencedOrigin
	);
	~FlowNode() override = default;

protected:
	FlowInputPort m_flowInputPort;
	std::tuple<VariableInputPort<InputTypes>...> m_variableInputPorts;
	FlowOutputPort m_flowOutputPort;

public:
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager) override;
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager) override;
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager) override;
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager) override;

protected:
	virtual void SetFocused(const bool& isFocused) override;
	virtual ImVec2 GetInternalNodeSize() override;

public:
	void Execute();
	std::tuple<InputTypes...> GetVariables() { return GetVariablesFromInput(m_variableInputPorts, std::index_sequence_for<InputTypes...>{}); }

protected:
	virtual void ExecuteImpl() = 0;
};

template<typename ...InputTypes>
inline FlowNode<InputTypes...>::FlowNode(const std::string& nodeName, const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin)
	: Node(nodeName, leftTop, referencedOrigin, flowTypeColor),
	m_variableInputPorts(CreateVariableInputPorts<InputTypes...>(this, radius, referencedOrigin, 1, std::index_sequence_for<InputTypes...>{})),
	m_flowInputPort(this, sizeof...(InputTypes) + 1, 0, radius, referencedOrigin),
	m_flowOutputPort(this, 1, 0, radius, referencedOrigin)
{

}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::AddToDrawElementManager(drawElementManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.AddToDrawElementManager(drawElementManager)); }, m_variableInputPorts);
	m_flowInputPort.AddToDrawElementManager(drawElementManager);
	m_flowOutputPort.AddToDrawElementManager(drawElementManager);
}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::RemoveFromDrawElementManager(drawElementManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.RemoveFromDrawElementManager(drawElementManager)); }, m_variableInputPorts);
	m_flowInputPort.RemoveFromDrawElementManager(drawElementManager);
	m_flowOutputPort.RemoveFromDrawElementManager(drawElementManager);
}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::RegisterToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.RegisterToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_flowInputPort.RegisterToInteractionManager(interactionManager);
	m_flowOutputPort.RegisterToInteractionManager(interactionManager);
}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::DeregisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::DeregisterToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.DeregisterToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_flowInputPort.DeregisterToInteractionManager(interactionManager);
	m_flowOutputPort.DeregisterToInteractionManager(interactionManager);
}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::SetFocused(const bool& isFocused)
{
	Node::SetFocused(isFocused);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.SetFocused(isFocused)); }, m_variableInputPorts);
	m_flowInputPort.SetFocused(isFocused);
	m_flowOutputPort.SetFocused(isFocused);
}

template<typename ...InputTypes>
inline ImVec2 FlowNode<InputTypes...>::GetInternalNodeSize()
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
		std::max(totalHeight * 2.f + m_flowInputPort.GetRadius() + m_flowOutputPort.GetRadius(), nodeMinHeight)
	);
}

template<typename ...InputTypes>
inline void FlowNode<InputTypes...>::Execute()
{
	FlowOutputPort* connectedOutputPort = m_flowInputPort.GetConnectedOutputPort();

	if (connectedOutputPort != nullptr)
	{
		auto* parentFlowNode = connectedOutputPort->GetParentFlowNode();

		if (parentFlowNode != nullptr)
		{
			parentFlowNode->Execute();
		}
	}
	ExecuteImpl();
}
