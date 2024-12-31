#include "pch.h"
#include "ScratchTextureAsset.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"

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
	), m_format(metaData.format)
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
			exception assetException("ScratchImage::GetImage's Result (DirectX::Image) is nullptr");
			throw assetException;
		}
	}
}

vector<uint32_t> ScratchTextureAsset::GetRowPitchArray()
{
	return m_rowPitchPerArray;
}

void ScratchTextureAsset::InitializeGPUAsset(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext
)
{
	m_resource = new Texture2DInstance<SRVOption, RTVOption>(
		m_width, m_height, m_arraySize, m_imageBuffers, GetRowPitchArray(),
		NULL, D3D11_RESOURCE_MISC_GENERATE_MIPS, D3D11_USAGE_DEFAULT, m_format,
		device, deviceContext
	);
}

void ScratchTextureAsset::Serialize(FILE* fileIn) const
{
	ATextureAsset::Serialize(fileIn);
	SerializeHelper::SerializeElement(m_format, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_rowPitchPerArray, fileIn);
}

void ScratchTextureAsset::Deserialize(FILE* fileIn)
{
	ATextureAsset::Deserialize(fileIn);
	m_format = DeserializeHelper::DeserializeElement<DXGI_FORMAT>(fileIn);
	m_rowPitchPerArray = DeserializeHelper::DeserializeSequenceContainer<vector<UINT>>(fileIn);
}

void ScratchTextureAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
