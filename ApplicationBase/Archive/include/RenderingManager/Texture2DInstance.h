#pragma once
#include "ITextureOption.h"
#include "DirectXUtilities.h"
#include <vector>

template<typename ...IsTextureOption>
class Texture2DInstance : public IsTextureOption...
{
public:
	Texture2DInstance(
		const UINT& width, 
		const UINT& height,
		const UINT& arraySize,
		const std::vector<std::vector<uint8_t>>& textureDataPerArray,
		const std::vector<UINT>& textureRowPitchPerArray,
		const UINT& cpuAccessFlag,
		const UINT& miscFlagIn,
		const D3D11_USAGE& usage,
		const DXGI_FORMAT& format,
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext
	);

public:
	static D3D11_BIND_FLAG GetBindFlags()
	{
		return (D3D11_BIND_FLAG)(IsTextureOption::GetBindFlag() | ...);
	};

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;

public:
	inline const ID3D11Texture2D* const GetTexture2D() { return m_texture2D.Get(); }

private:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device) override;
};

template<typename ...IsTextureOption>
inline Texture2DInstance<IsTextureOption...>::Texture2DInstance(
	const UINT& width,
	const UINT& height,
	const UINT& arraySize,
	const std::vector<std::vector<uint8_t>>& textureDataPerArray,
	const std::vector<UINT>& textureRowPitchPerArray,
	const UINT& cpuAccessFlag,
	const UINT& miscFlagIn,
	const D3D11_USAGE& usage,
	const DXGI_FORMAT& format,
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext
)
{
	D3D11_TEXTURE2D_DESC texture2DDesc;
	AutoZeroMemory(texture2DDesc);
	texture2DDesc.Width = width;
	texture2DDesc.Height = height;
	texture2DDesc.ArraySize = arraySize;
	texture2DDesc.MipLevels = 0;
	texture2DDesc.BindFlags = Texture2DInstance<IsTextureOption...>::GetBindFlags();
	texture2DDesc.CPUAccessFlags = cpuAccessFlag;
	texture2DDesc.MiscFlags = miscFlagIn;
	texture2DDesc.SampleDesc.Count = 1;
	texture2DDesc.SampleDesc.Quality = 0;
	texture2DDesc.Usage = usage;
	texture2DDesc.Format = format;
	AssertIfFailed(device->CreateTexture2D(&texture2DDesc, NULL, m_texture2D.GetAddressOf()));

	for (size_t arrayIdx = 0; arrayIdx < textureDataPerArray.size(); ++arrayIdx)
	{
		const uint8_t* imageBuffer = textureDataPerArray[arrayIdx].data();
		deviceContext->UpdateSubresource(
			m_texture2D.Get(), D3D11CalcSubresource(0, static_cast<UINT>(arrayIdx), texture2DDesc.MipLevels), nullptr,
			imageBuffer, textureRowPitchPerArray[arrayIdx], NULL
		);
	}

	InitializeByOption(m_texture2D.Get(), device);
}

template<typename ...IsTextureOption>
inline void Texture2DInstance<IsTextureOption...>::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device)
{
	(IsTextureOption::InitializeByOption(resource, device), ...);
}

