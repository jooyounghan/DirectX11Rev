#pragma once
#include "Port.h"

template<typename T>
class VariablePort : public Port
{
public:
	VariablePort(
		Node* parentNode, const bool& isLeft,
		size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariablePort() override = default;
};

template<typename T>
inline VariablePort<T>::VariablePort(
	Node* parentNode, const bool& isLeft, 
	size_t indexCount, size_t portIndex,
	const float& radius, const ImVec2& referencedOrigin
)
	: Port(parentNode, isLeft, indexCount, portIndex, radius, referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
	)
{
};