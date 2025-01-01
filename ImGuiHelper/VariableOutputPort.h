#pragma once
#include "VariablePort.h"
#include <type_traits>

template <typename OutputType>
class VariableInputPort;

template<typename OutputType>
class VariableOutputPort : public VariablePort<OutputType>
{
public:
	VariableOutputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariableOutputPort() override = default;

public:
	template<typename InputType>
	bool IsConnectable(VariableInputPort<InputType>* variableInputPort);

public:
	virtual OutputType GetVariable() const { return OutputType(); }
};

template<typename OutputType>
inline VariableOutputPort<OutputType>::VariableOutputPort(
	Node* parentNode, size_t indexCount,
	size_t portIndex, const float& radius, const ImVec2& referencedOrigin
)
	: VariablePort<OutputType>(parentNode, false, indexCount, portIndex, radius, referencedOrigin)
{
}

template<typename OutputType>
template<typename InputType>
inline bool VariableOutputPort<OutputType>::IsConnectable(VariableInputPort<InputType>* variableInputPort)
{
	return std::is_base_of_v<OutputType, InputType>();
}
