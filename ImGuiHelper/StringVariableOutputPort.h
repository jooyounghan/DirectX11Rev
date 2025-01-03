#pragma once
#include "VariableOutputPort.h"

class StringVariableOutputPort : public VariableOutputPort<std::string>
{
public:
	StringVariableOutputPort(
		Node* parentNode, size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~StringVariableOutputPort() override = default;

protected:
	std::string m_stringValue;

public:
	inline void SetString(const std::string& stringValue) { m_stringValue = stringValue; }

public:
	virtual std::string GetVariable() const override;
};

