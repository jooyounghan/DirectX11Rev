#pragma once
template <typename T>
class IVariableOutputPort
{
public:
    IVariableOutputPort() = default;
    virtual ~IVariableOutputPort() = default;

public:
    virtual T GetVariable() const = 0;
};

