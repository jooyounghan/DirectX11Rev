#include "IAssetFile.h"

using namespace std;

IAssetFile::IAssetFile(
	const string& AssetNameIn, EAssetType AssetTypeIn
)
	: AssetName(AssetNameIn), AssetType(AssetTypeIn)
{
}

IAssetFile::~IAssetFile()
{
}
