#pragma once
#include "Port.h"

#include <tuple>
#include <array>

template <typename OutputType>
class IVariableOutputPort;

template <typename... InputTypes>
class IVariableInputPort
{
public:
    using ConnectedOutputPorts = std::tuple<IVariableOutputPort<InputTypes>*...>;

public:
    IVariableInputPort(Node* node, const float& radius, const ImVec2& referencedOrigin);
    virtual ~IVariableInputPort();

public:
    static constexpr size_t GetInputCount() { return sizeof...(InputTypes); }
    std::tuple<InputTypes...> GetVariables() const;
    void SetConnectedOutputPorts(const ConnectedOutputPorts& conntectedOutputPorts);

protected:
    static constexpr size_t InputCounts = IVariableInputPort<InputTypes...>::GetInputCount();

protected:
    ConnectedOutputPorts m_conntectedOutputPorts;
    std::array<Port*, InputCounts> m_inputPorts;

private:
    template <size_t... Indices>
    std::tuple<InputTypes...> GetVariablesImpl(std::index_sequence<Indices...>) const;

    template <size_t... Indices>
    void DeleteInputPortsImpl(std::index_sequence<Indices...>);

    template <size_t... Indices>
    std::array<Port*, sizeof...(InputTypes)> GetInputPortsImpl(Node* node, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>);
};

template<typename ...InputTypes>
inline IVariableInputPort<InputTypes...>::IVariableInputPort(Node* node, const float& radius, const ImVec2& referencedOrigin)
    : m_inputPorts(GetInputPortsImpl(node, radius, referencedOrigin, std::index_sequence_for<InputTypes...>{}))
{
}

template<typename ...InputTypes>
inline IVariableInputPort<InputTypes...>::~IVariableInputPort()
{
    DeleteInputPortsImpl(std::index_sequence_for<InputTypes...>{});
}

template<typename ...InputTypes>
inline std::tuple<InputTypes...> IVariableInputPort<InputTypes...>::GetVariables() const
{
    return GetVariablesImpl(std::index_sequence_for<InputTypes...>{});
}

template<typename ...InputTypes>
inline void IVariableInputPort<InputTypes...>::SetConnectedOutputPorts(const ConnectedOutputPorts& conntectedOutputPorts)
{
    m_conntectedOutputPorts = conntectedOutputPorts;
}

template<typename ...InputTypes>
template<size_t ...Indices>
inline std::tuple<InputTypes...> IVariableInputPort<InputTypes...>::GetVariablesImpl(std::index_sequence<Indices...>) const
{
    return std::make_tuple(std::get<Indices>(m_conntectedOutputPorts)->GetVariable()...);
}

template<typename ...InputTypes>
template<size_t ...Indices>
inline void IVariableInputPort<InputTypes...>::DeleteInputPortsImpl(std::index_sequence<Indices...>)
{
    (..., delete m_inputPorts[Indices]);
}

template<typename ...InputTypes>
template<size_t ...Indices>
inline std::array<Port*, sizeof...(InputTypes)> IVariableInputPort<InputTypes...>::GetInputPortsImpl(Node* node, const float& radius, const ImVec2& referencedOrigin, std::index_sequence<Indices...>)
{
    return { (
        new Port(node, true, sizeof...(InputTypes), Indices, radius, referencedOrigin,
            IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
            IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
        )
        )... };
}
