#pragma once
#include "IGPUAsset.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"
#include <vector>

class ATextureGPUAsset : public IGPUAsset
{
public:
	ATextureGPUAsset() = default;
	virtual ~ATextureGPUAsset();

public:
	inline ID3D11Texture2D* const GetTexture2D() { return m_resource->GetTexture2D(); };
	inline ID3D11ShaderResourceView* const GetSRV() { return m_resource->GetSRV(); };
	inline ID3D11RenderTargetView* const GetRTV() { return m_resource->GetRTV(); };

protected:
	Texture2DInstance<SRVOption, RTVOption>* m_resource = nullptr;
};

