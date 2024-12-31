#pragma once
#include <tuple>

template <typename T>
class IVariableOutputPort;

template <typename... Args>
class IVariableInputPort
{
public:
    using ConnectedOutputPorts = std::tuple<IVariableOutputPort<Args>*...>;

public:
    IVariableInputPort() = default;
    virtual ~IVariableInputPort() = default;

public:
    static constexpr size_t GetInputCount() { return sizeof...(Args); }
    std::tuple<Args...> GetVariables() const;
    void SetConnectedOutputPorts(const ConnectedOutputPorts& conntectedOutputPorts);

protected:
    ConnectedOutputPorts m_conntectedOutputPorts;

private:
    template <size_t... Indices>
    std::tuple<Args...> GetVariablesImpl(std::index_sequence<Indices...>) const;
};

template<typename ...Args>
inline std::tuple<Args...> IVariableInputPort<Args...>::GetVariables() const
{
    return GetVariablesImpl(std::index_sequence_for<Args...>{});
}

template<typename ...Args>
inline void IVariableInputPort<Args...>::SetConnectedOutputPorts(const ConnectedOutputPorts& conntectedOutputPorts)
{
    m_conntectedOutputPorts = conntectedOutputPorts;
}

template<typename ...Args>
template<size_t ...Indices>
inline std::tuple<Args...> IVariableInputPort<Args...>::GetVariablesImpl(std::index_sequence<Indices...>) const
{
    return std::make_tuple(std::get<Indices>(m_conntectedOutputPorts)->GetVariable()...);
}
