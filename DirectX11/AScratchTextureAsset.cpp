#include "AScratchTextureAsset.h"

#include "zlib.h"

using namespace std;
using namespace DirectX;

AScratchTextureAsset::AScratchTextureAsset(
	const string& AssetNameIn,
	const string& AssetTypeIn
)
	: ATextureAsset(AssetNameIn, AssetTypeIn)
{
}


AScratchTextureAsset::AScratchTextureAsset(
	const string& AssetNameIn,
	const string& AssetTypeIn,
	const ScratchImage& scratch, 
	const TexMetadata& metaData
)
	: ATextureAsset(AssetNameIn, AssetTypeIn)
{
}

vector<vector<uint8_t>> AScratchTextureAsset::CompressDataArray(
	const vector<uint8_t*>& DecompressedBufferPerArrayIn,
	const vector<size_t>& OriginalSizePerArray,
	vector<size_t>& CompressedSizePerArrayOut
)
{
	vector<vector<uint8_t>> vResult;
	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{

		uLong CompressedSize = compressBound(OriginalSizePerArray[ArrayIdx]);
		vector<uint8_t> DecompressedData(CompressedSize);

		int Result = compress2(
			DecompressedData.data(), &CompressedSize,
			DecompressedBufferPerArrayIn[ArrayIdx],
			OriginalSizePerArray[ArrayIdx], Z_BEST_COMPRESSION
		);
		DecompressedData.resize(CompressedSize);
		CompressedSizePerArrayOut[ArrayIdx] = CompressedSize;

		vResult.push_back(move(DecompressedData));
	}

	return vResult;
}

vector<vector<uint8_t>> AScratchTextureAsset::DecompressDataArray(
	const vector<uint8_t*>& CompressedBufferPerArrayIn,
	const vector<size_t>& CompressedSizePerArray,
	const vector<size_t>& OriginalSizePerArray
)
{
	vector<vector<uint8_t>> vResult;
	for (size_t ArrayIdx = 0; ArrayIdx < ArraySize; ++ArrayIdx)
	{
		uLongf DecompressedSize = OriginalSizePerArray[ArrayIdx];
		vector<uint8_t> DecompressedData(OriginalSizePerArray[ArrayIdx]);

		int Result = uncompress(
			DecompressedData.data(),
			&DecompressedSize,
			CompressedBufferPerArrayIn[ArrayIdx],
			CompressedSizePerArray[ArrayIdx]
		);
		vResult.push_back(move(DecompressedData));
	}
	return vResult;
}
