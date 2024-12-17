#include "pch.h"
#include "ATextureAsset.h"

using namespace std;

ATextureAsset::ATextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& arraysizeIn
) 
	: AAsset(assetName), m_width(widthIn), m_height(heightIn), m_arraySize(arraysizeIn)
{

}

ATextureAsset::~ATextureAsset() 
{

}




void ATextureAsset::Serialize(FILE* fileIn) const
{

}



void ATextureAsset::Deserialize(FILE* fileIn)
{

}