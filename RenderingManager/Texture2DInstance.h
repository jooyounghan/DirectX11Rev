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
		const UINT& mipLevels,
		const UINT& cpuAccessFlag,
		const UINT& miscFlagIn,
		const D3D11_USAGE& usage,
		const DXGI_FORMAT& format,
		const UINT& additionalBindFlag = NULL
	);

public:
	static D3D11_BIND_FLAG GetBindFlags()
	{
		return (D3D11_BIND_FLAG)(IsTextureOption::GetBindFlag() | ...);
	};

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
	D3D11_TEXTURE2D_DESC m_texture2DDesc;

public:
	inline ID3D11Texture2D* const GetTexture2D() const { return m_texture2D.Get(); }

public:
	void UpdateTextureByBuffer(
		const std::vector<std::vector<uint8_t>>& textureDataPerArray,
		const std::vector<UINT>& textureRowPitchPerArray,
		ID3D11DeviceContext* deviceContext
	);
	virtual void InitializeByOption(
		ID3D11Device* device, 
		ID3D11DeviceContext* deviceContext,
		ID3D11Resource* resource = nullptr 
	) override;
};

template<typename ...IsTextureOption>
inline Texture2DInstance<IsTextureOption...>::Texture2DInstance(
	const UINT& width,
	const UINT& height,
	const UINT& arraySize,
	const UINT& mipLevels,
	const UINT& cpuAccessFlag,
	const UINT& miscFlagIn, 
	const D3D11_USAGE& usage,
	const DXGI_FORMAT& format,
	const UINT& additionalBindFlag
)
{
	AutoZeroMemory(m_texture2DDesc);
	m_texture2DDesc.Width = width;
	m_texture2DDesc.Height = height;
	m_texture2DDesc.ArraySize = arraySize;
	m_texture2DDesc.MipLevels = mipLevels;
	m_texture2DDesc.BindFlags = (D3D11_BIND_FLAG)(additionalBindFlag | Texture2DInstance<IsTextureOption...>::GetBindFlags());
	m_texture2DDesc.CPUAccessFlags = cpuAccessFlag;
	m_texture2DDesc.MiscFlags = miscFlagIn;
	m_texture2DDesc.SampleDesc.Count = 1;
	m_texture2DDesc.SampleDesc.Quality = 0;
	m_texture2DDesc.Usage = usage;
	m_texture2DDesc.Format = format;
}


template<typename ...IsTextureOption>
inline void Texture2DInstance<IsTextureOption...>::UpdateTextureByBuffer(
	const std::vector<std::vector<uint8_t>>& textureDataPerArray,
	const std::vector<UINT>& textureRowPitchPerArray,
	ID3D11DeviceContext* deviceContext
)
{
	for (size_t arrayIdx = 0; arrayIdx < textureDataPerArray.size(); ++arrayIdx)
	{
		const uint8_t* imageBuffer = textureDataPerArray[arrayIdx].data();
		deviceContext->UpdateSubresource(
			m_texture2D.Get(), D3D11CalcSubresource(0, static_cast<UINT>(arrayIdx), m_texture2DDesc.MipLevels),
			nullptr, imageBuffer, textureRowPitchPerArray[arrayIdx], NULL
		);
	}
}

template<typename ...IsTextureOption>
inline void Texture2DInstance<IsTextureOption...>::InitializeByOption(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	ID3D11Resource* resource
)
{
	AssertIfFailed(device->CreateTexture2D(&m_texture2DDesc, NULL, m_texture2D.GetAddressOf()));
	(IsTextureOption::InitializeByOption(device, deviceContext, m_texture2D.Get()), ...);
}

