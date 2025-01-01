#pragma once
#include "VariablePort.h"
#include <type_traits>

template <typename OutputType>
class VariableOutputPort;

template<typename InputType>
class VariableInputPort : public VariablePort<InputType>
{
public:
	VariableInputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariableInputPort() override = default;

protected:
	VariableOutputPort<InputType>* m_connectedPort;

public:
	template<typename OutputType>
	bool IsConnectable(VariableOutputPort<OutputType>* variableOutputPort);
};


template<typename InputType>
inline VariableInputPort<InputType>::VariableInputPort(
	Node* parentNode, size_t indexCount, 
	size_t portIndex, const float& radius, const ImVec2& referencedOrigin
)
	: VariablePort<InputType>(parentNode, true, indexCount, portIndex, radius, referencedOrigin)
{
}

template<typename InputType>
template<typename OutputType>
inline bool VariableInputPort<InputType>::IsConnectable(VariableOutputPort<OutputType>* variableOutputPort)
{
	return std::is_base_of_v<OutputType, InputType>();
}
