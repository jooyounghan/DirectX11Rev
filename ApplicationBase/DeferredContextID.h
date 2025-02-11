#pragma once

enum class EDeferredContextType
{
	ASSETS_LOAD,
	COMPONENT_UPDATE,
	COMPONENT_RENDER
};

class DeferredContextID
{
public:
    constexpr DeferredContextID(EDeferredContextType DeferredContextType) : m_DeferredContextType(DeferredContextType) {}

private:
    EDeferredContextType m_DeferredContextType;

public:
    constexpr operator size_t() const 
    {
        return static_cast<size_t>(m_DeferredContextType);
    }
};