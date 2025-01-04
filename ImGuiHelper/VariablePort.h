#pragma once
#include "APort.h"
#include "DrawElementColor.h"

#include <string>

std::string GetBaseTypeName(std::string typeName);

template<typename T>
class VariablePort : public APort
{
public:
	VariablePort(
		Node* parentNode, const bool& isLeft,
		size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariablePort() override = default;

protected:
	static VariablePort<T>* m_connectingInputPort;
	static VariablePort<T>* m_connectingOutputPort;

public:
	template<typename U>
	bool IsConnectable(VariablePort<U>* targetPort);
};

template <typename T>
VariablePort<T>* VariablePort<T>::m_connectingInputPort = nullptr;

template <typename T>
VariablePort<T>* VariablePort<T>::m_connectingOutputPort = nullptr;

template<typename T>
inline VariablePort<T>::VariablePort(
	Node* parentNode, const bool& isLeft, 
	size_t indexCount, size_t portIndex,
	const float& radius, const ImVec2& referencedOrigin
)
	: APort(parentNode, isLeft, indexCount, portIndex, radius, referencedOrigin, variableTypeColor, variableTypeHilightColor)
{
}

template<typename T>
template<typename U>
inline bool VariablePort<T>::IsConnectable(VariablePort<U>* targetPort)
{
	return std::is_same_v<U, T> || std::is_base_of_v<U, T>;
}
