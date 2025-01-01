#pragma once
#include "Port.h"

template <typename OutputType>
class IVariableOutputPort
{
public:
    IVariableOutputPort(Node* node, const float& radius, const ImVec2& referencedOrigin);
    virtual ~IVariableOutputPort();

protected:
    Port* m_outputPort;

public:
    virtual OutputType GetVariable() const = 0;
};

template<typename T>
inline IVariableOutputPort<T>::IVariableOutputPort(Node* node, const float& radius, const ImVec2& referencedOrigin)
    : m_outputPort(new Port(node, false, 1, 0, radius, referencedOrigin,
        IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
        IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)))
{

}

template<typename T>
inline IVariableOutputPort<T>::~IVariableOutputPort()
{
    delete m_outputPort;
}
