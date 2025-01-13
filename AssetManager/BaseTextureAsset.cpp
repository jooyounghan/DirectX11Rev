#include "BaseTextureAsset.h"

using namespace std;

BaseTextureAsset::BaseTextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	uint8_t* imageBufferIn
)
	: ATextureAsset(assetName, widthIn, heightIn, 1)
{
	vector<uint8_t> imageBuffer;
	imageBuffer.resize(m_width * m_height * 4);
	memcpy(imageBuffer.data(), imageBufferIn, imageBuffer.size());

	m_imageBuffers.emplace_back(move(imageBuffer));
}

vector<uint32_t> BaseTextureAsset::GetRowPitchArray()
{
	return { m_width * 4 };
}

void BaseTextureAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
