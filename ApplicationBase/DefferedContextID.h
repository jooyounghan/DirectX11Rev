#pragma once

enum class EDefferedContextType
{
	ASSETS_LOAD,
	COMPONENT_UPDATE,
	COMPONENT_RENDER
};

class DefferedContextID
{
public:
    constexpr DefferedContextID(EDefferedContextType defferedContextType) : m_defferedContextType(defferedContextType) {}

private:
    EDefferedContextType m_defferedContextType;

public:
    constexpr operator size_t() const 
    {
        return static_cast<size_t>(m_defferedContextType);
    }
};