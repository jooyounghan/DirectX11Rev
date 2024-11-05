#pragma once
#include <string>

struct ID3D11Device;
class AssetManager;

class ISerializable
{
public:
	virtual void Serialize() = 0;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

class IOnSerializableElement
{
public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};