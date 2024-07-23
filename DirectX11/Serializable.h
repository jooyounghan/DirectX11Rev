#pragma once
#include <string>

struct ID3D11Device;

class ISerializable
{
public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) = 0;
};

