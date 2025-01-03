#include "StringVariableOutputPort.h"

using namespace std;

StringVariableOutputPort::StringVariableOutputPort(Node* parentNode, size_t indexCount, size_t portIndex, const float& radius, const ImVec2& referencedOrigin)
	: VariableOutputPort<string>(parentNode, indexCount, portIndex, radius, referencedOrigin)
{
}

string StringVariableOutputPort::GetVariable() const
{
	return m_stringValue;
}
