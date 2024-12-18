#include "pch.h"
#include "BaseTextureAsset.h"

using namespace std;

BaseTextureAsset::BaseTextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& channelIn,
	uint8_t* imageBufferIn
)
	: ATextureAsset(assetName, widthIn, heightIn, 1), m_channel(channelIn)
{
	vector<uint8_t> imageBuffer;
	imageBuffer.resize(m_width * m_height * m_channel);
	memcpy(imageBuffer.data(), imageBufferIn, imageBuffer.size());

	m_imageBuffers.emplace_back(move(imageBuffer));
}

BaseTextureAsset::~BaseTextureAsset() {}

vector<uint32_t> BaseTextureAsset::GetRowPitchArray()
{
	return { m_width * m_channel };
}

void BaseTextureAsset::InitializeGPUAsset(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext
)
{
	m_resource = new Texture2DInstance<SRVOption, RTVOption>(
		m_width, m_height, 1, m_imageBuffers, GetRowPitchArray(),
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM,
		device, deviceContext
	);
}

void BaseTextureAsset::Serialize(FILE* fileIn) const
{
	ATextureAsset::Serialize(fileIn);
	SerializeHelper::SerializeElement(m_channel, fileIn);
}

void BaseTextureAsset::Deserialize(FILE* fileIn)
{
	ATextureAsset::Deserialize(fileIn);
	m_channel = DeserializeHelper::DeserializeElement<UINT>(fileIn);
}

void BaseTextureAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
