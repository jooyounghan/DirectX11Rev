#pragma once
#include "IVariableInputPort.h"
#include "IVariableOutputPort.h"
#include "Node.h"
#include <functional>

template<typename OutputType, typename ...InputTypes>
class VariableNode : public Node, public IVariableInputPort<InputTypes...>, public IVariableOutputPort<OutputType>
{
	using OutputPort = IVariableOutputPort<OutputType>;
	using InputPorts = IVariableInputPort<InputTypes...>;

public:
	VariableNode(const ImVec2& leftTop, const ImVec2& size, const float& radius, const ImVec2& referencedOrigin);
	~VariableNode() override = default;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;

public:
	virtual OutputType GetVariable() const { return OutputType{}; }
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);
};





template<typename OutputType, typename ...InputTypes>
inline VariableNode<OutputType, InputTypes...>::VariableNode(
	const ImVec2& leftTop, const ImVec2& size, const float& radius, const ImVec2& referencedOrigin
)
	: Node(leftTop, size, referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
	), InputPorts(this, radius, referencedOrigin),
	OutputPort(this, radius, referencedOrigin)
{
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::DrawImpl(ImDrawList* drawListIn)
{
	Node::DrawImpl(drawListIn);
	for (Port* inputPort : InputPorts::m_inputPorts)
	{
		inputPort->Draw(drawListIn);
	}
	OutputPort::m_outputPort->Draw(drawListIn);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::RegisterToInteractionManager(interactionManager);
	for (Port* inputPort : InputPorts::m_inputPorts)
	{
		inputPort->RegisterToInteractionManager(interactionManager);
	}
	OutputPort::m_outputPort->RegisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::DeregisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::DeregisterToInteractionManager(interactionManager);
	for (Port* inputPort : InputPorts::m_inputPorts)
	{
		inputPort->DeregisterToInteractionManager(interactionManager);
	}
	OutputPort::m_outputPort->DeregisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::AddToDrawElementManager(drawElementManager);
	for (Port* inputPort : InputPorts::m_inputPorts)
	{
		inputPort->AddToDrawElementManager(drawElementManager);
	}
	OutputPort::m_outputPort->AddToDrawElementManager(drawElementManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::RemoveFromDrawElementManager(drawElementManager);
	for (Port* inputPort : InputPorts::m_inputPorts)
	{
		inputPort->RemoveFromDrawElementManager(drawElementManager);
	}
	OutputPort::m_outputPort->RemoveFromDrawElementManager(drawElementManager);
}


//Control	IM_COL32(0x43, 0x58, 0xCC, 0x88), IM_COL32(0x22, 0x44, 0xFF, 0xFF)
//IM_COL32(0x4A, 0x4F, 0x66, 0x88), IM_COL32(0x51, 0x5C, 0x99, 0xFF)
//
//Flow	IM_COL32(0xCC, 0x58, 0x43, 0x88), IM_COL32(0xFF, 0x44, 0x22, 0xFF),
//IM_COL32(0x66, 0x4F, 0x4A, 0x88), IM_COL32(0x99, 0x5C, 0x51, 0xFF)
