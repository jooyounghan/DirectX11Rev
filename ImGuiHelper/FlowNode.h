#pragma once
#include "VariableNode.h"
#include "FlowInputPort.h"
#include "FlowOutputPort.h"
#include "NodeConstant.h"

template<typename ...InputTypes>
class FlowNode : public Node
{
public:
	FlowNode(
		const std::string& nodeName, const ImVec2& leftTop, const float& radius, 
		const ImVec2& referencedOrigin, 
		const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions
	);
	~FlowNode() override = default;

protected:
	FlowInputPort m_flowInputPort;
	std::tuple<VariableInputPort<InputTypes>...> m_variableInputPorts;
	FlowOutputPort m_flowOutputPort;

public:
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager) override;
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager) override;
	virtual void BringFrontToInteractionManager(InteractionManager* interactionManager);

protected:
	virtual void SetFocused(const bool& isFocused) override;
	virtual void Draw(ImDrawList* drawListIn) override final;

public:
	void Execute();
	std::tuple<InputTypes...> GetInputVariables() { return GetVariablesFromInput(m_variableInputPorts, std::index_sequence_for<InputTypes...>{}); }
	ImVec2 GetMinNodeSize();

protected:
	virtual void ExecuteImpl() = 0;
};

template<typename ...InputTypes>
inline FlowNode<InputTypes...>::FlowNode(
	const std::string& nodeName, const ImVec2& leftTop, const float& radius, const ImVec2& referencedOrigin,
	const std::array<std::string, sizeof...(InputTypes)>& inputDescriptions
)
	: Node(nodeName, leftTop, referencedOrigin, flowTypeColor),
	m_variableInputPorts(CreateVariableInputPorts<InputTypes...>(this, radius, referencedOrigin, 1, inputDescriptions, std::index_sequence_for<InputTypes...>{})),
	m_flowInputPort(this, sizeof...(InputTypes) + 1, 0, radius, referencedOrigin),
	m_flowOutputPort(this, 1, 0, radius, referencedOrigin)
{

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
inline void FlowNode<InputTypes...>::BringFrontToInteractionManager(InteractionManager* interactionManager)
{
	Node::BringFrontToInteractionManager(interactionManager);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.BringFrontToInteractionManager(interactionManager)); }, m_variableInputPorts);
	m_flowInputPort.BringFrontToInteractionManager(interactionManager);
	m_flowOutputPort.BringFrontToInteractionManager(interactionManager);
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
inline void FlowNode<InputTypes...>::Draw(ImDrawList* drawListIn)
{
	Node::Draw(drawListIn);
	std::apply([&](auto&... inputPorts) { (..., inputPorts.Draw(drawListIn)); }, m_variableInputPorts);
	m_flowInputPort.Draw(drawListIn);
	m_flowOutputPort.Draw(drawListIn);
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

template<typename ...InputTypes>
inline ImVec2 FlowNode<InputTypes...>::GetMinNodeSize()
{
	ImVec2 minNodeSize = ImVec2(0.f, 0.f);
	minNodeSize.y += (m_flowInputPort.GetRadius() * 2.f + nodeInternalMargin);
	std::apply([&](auto&... variableInputPorts) 
		{
			(..., [&]() 
				{
					const ImVec2 typeTextSize = variableInputPorts.GetTypeTextSize();
					minNodeSize.x = std::max(minNodeSize.x, variableInputPorts.GetRadius() * 2.f + nodeInternalMargin + typeTextSize.x);
					minNodeSize.y += (typeTextSize.y + nodeInternalMargin);
				}());
		}, m_variableInputPorts
	);

	return minNodeSize;
}