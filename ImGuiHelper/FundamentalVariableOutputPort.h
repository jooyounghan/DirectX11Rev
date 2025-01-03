#pragma once
#include "VariableOutputPort.h"
#include <type_traits>

template<typename T>
concept IsFundamental = std::is_fundamental_v<T>;

template<IsFundamental T>
class FundamentalVariableOutputPort : public VariableOutputPort<T>
{
public:
	FundamentalVariableOutputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~FundamentalVariableOutputPort() override = default;

protected:
	T m_fundimentalValue = T();

public:
	inline T* GetVariableAddress() { return &m_fundimentalValue; }

public:
	virtual T GetVariable() const override;
};

template<IsFundamental T>
inline FundamentalVariableOutputPort<T>::FundamentalVariableOutputPort(
	Node* parentNode, size_t indexCount, size_t portIndex, 
	const float& radius, const ImVec2& referencedOrigin
)
	: VariableOutputPort<T>(parentNode, indexCount, portIndex, radius, referencedOrigin)
{
}

template<IsFundamental T>
inline T FundamentalVariableOutputPort<T>::GetVariable() const
{
	return m_fundimentalValue;
}


