#pragma once
#include <string>

struct ID3D11Device;
class AssetManager;

class ISerializable
{
public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

class IOnSerializableMap
{
public:
	virtual void OnSerializeFromMap(FILE* FileIn) = 0;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};