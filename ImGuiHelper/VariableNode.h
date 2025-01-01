#pragma once
#include "IVariableInputPort.h"
#include "IVariableOutputPort.h"
#include "Node.h"
#include "Port.h"
#include <array>
#include <functional>

template<typename OutputType, typename ...InputTypes>
class VariableNode : public Node, public IVariableInputPort<InputTypes...>, public IVariableOutputPort<OutputType>
{
public:
	VariableNode(
		const ImVec2& leftTop, const ImVec2& size,
		const float& radius,
		const ImVec2& referencedOrigin
	);
	~VariableNode() override
	{
		DeleteInputPortsImpl(std::index_sequence_for<InputTypes...>{});
		delete m_outputPorts;
	};

protected:
	static constexpr size_t InputCounts = IVariableInputPort<InputTypes...>::GetInputCount();
	std::array<Port*, InputCounts> m_inputPorts;
	Port* m_outputPorts;

protected:
	virtual void DrawImpl(ImDrawList* drawListIn) override;

private:
	template <size_t... Indices>
	void DeleteInputPortsImpl(std::index_sequence<Indices...>)
	{
		(..., delete m_inputPorts[Indices]);
	}

	template <size_t... Indices>
	std::array<Port*, sizeof...(InputTypes)> GetInputPortsImpl(const float& radius, std::index_sequence<Indices...>)
	{
		return { (
			new Port(this, true, sizeof...(InputTypes), Indices, radius, m_referencedOrigin,
				IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
				IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
			)
		)... };
	}

public:
	virtual OutputType GetVariable() const { return OutputType{}; }

public:
	virtual void RegisterToInteractionManager(InteractionManager* interactionManager);
	virtual void DeregisterToInteractionManager(InteractionManager* interactionManager);
	virtual void AddToDrawElementManager(DrawElementManager* drawElementManager);
	virtual void RemoveFromDrawElementManager(DrawElementManager* drawElementManager);

private:
	void Test(Port* port) 
	{
		bool test = true;
	}
};

template<typename OutputType, typename ...InputTypes>
inline VariableNode<OutputType, InputTypes...>::VariableNode(
	const ImVec2& leftTop, const ImVec2& size, const float& radius, const ImVec2& referencedOrigin
)
	: Node(leftTop, size, referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
	), m_inputPorts(GetInputPortsImpl(radius, std::index_sequence_for<InputTypes...>{}))
	, m_outputPorts(new Port(this, false, 1, 0, radius, m_referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)))
{
	m_outputPorts->OnQueryConnectionHandler = std::bind(&VariableNode<OutputType, InputTypes...>::Test, this, std::placeholders::_1);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::DrawImpl(ImDrawList* drawListIn)
{
	Node::DrawImpl(drawListIn);
	for (Port* inputPort : m_inputPorts)
	{
		inputPort->Draw(drawListIn);
	}
	m_outputPorts->Draw(drawListIn);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::RegisterToInteractionManager(interactionManager);
	for (Port* inputPort : m_inputPorts)
	{
		inputPort->RegisterToInteractionManager(interactionManager);
	}
	m_outputPorts->RegisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::DeregisterToInteractionManager(InteractionManager* interactionManager)
{
	Node::DeregisterToInteractionManager(interactionManager);
	for (Port* inputPort : m_inputPorts)
	{
		inputPort->DeregisterToInteractionManager(interactionManager);
	}
	m_outputPorts->DeregisterToInteractionManager(interactionManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::AddToDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::AddToDrawElementManager(drawElementManager);
	for (Port* inputPort : m_inputPorts)
	{
		inputPort->AddToDrawElementManager(drawElementManager);
	}
	m_outputPorts->AddToDrawElementManager(drawElementManager);
}

template<typename OutputType, typename ...InputTypes>
inline void VariableNode<OutputType, InputTypes...>::RemoveFromDrawElementManager(DrawElementManager* drawElementManager)
{
	Node::RemoveFromDrawElementManager(drawElementManager);
	for (Port* inputPort : m_inputPorts)
	{
		inputPort->RemoveFromDrawElementManager(drawElementManager);
	}
	m_outputPorts->RemoveFromDrawElementManager(drawElementManager);
}


//Control	IM_COL32(0x43, 0x58, 0xCC, 0x88), IM_COL32(0x22, 0x44, 0xFF, 0xFF)
//IM_COL32(0x4A, 0x4F, 0x66, 0x88), IM_COL32(0x51, 0x5C, 0x99, 0xFF)
//
//Flow	IM_COL32(0xCC, 0x58, 0x43, 0x88), IM_COL32(0xFF, 0x44, 0x22, 0xFF),
//IM_COL32(0x66, 0x4F, 0x4A, 0x88), IM_COL32(0x99, 0x5C, 0x51, 0xFF)
