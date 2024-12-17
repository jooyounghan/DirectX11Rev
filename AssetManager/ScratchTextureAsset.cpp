#include "pch.h"
#include "ScratchTextureAsset.h"
#include "AssetException.h"

using namespace std;
using namespace DirectX;

ScratchTextureAsset::ScratchTextureAsset(
	const string& assetName, 
	const ScratchImage& scratch,
	const TexMetadata& metaData
)
	: ATextureAsset(
		assetName, 
		static_cast<unsigned int>(metaData.width), 
		static_cast<unsigned int>(metaData.height),
		static_cast<unsigned int>(metaData.arraySize)
	)
{
	for (size_t ArrayIdx = 0; ArrayIdx < m_arraySize; ++ArrayIdx)
	{
		const Image* img = scratch.GetImage(0, ArrayIdx, 0);

		if (img != nullptr)
		{
			const size_t originalBufferSize = img->rowPitch * img->height;
			vector<uint8_t> originalBuffer(originalBufferSize);
			memcpy(originalBuffer.data(), img->pixels, originalBufferSize);

			m_imageBuffers.emplace_back(move(originalBuffer));
			m_rowPitchPerArray.push_back(static_cast<UINT>(img->rowPitch));
		}
		else
		{
			AssetException assetException(*this, "ScratchImage::GetImage's Result (DirectX::Image) is nullptr");
			throw assetException;
		}
	}
}

ScratchTextureAsset::~ScratchTextureAsset()
{
}

vector<uint32_t> ScratchTextureAsset::GetRowPitchArray()
{
	return m_rowPitchPerArray;
}

const ID3D11Texture2D* const ScratchTextureAsset::GetTexture2D()
{
	return m_textureWithSRV->GetTexture2D();
}

const ID3D11ShaderResourceView* const ScratchTextureAsset::GetSRV()
{
	return m_textureWithSRV->GetSRV();
}

void ScratchTextureAsset::InitializeGPUAsset(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext
)
{
	m_textureWithSRV = new Texture2DInstance<SRVOption>(
		m_width, m_height, m_arraySize, m_imageBuffers, GetRowPitchArray(),
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM,
		device, deviceContext
	);
}

void ScratchTextureAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
