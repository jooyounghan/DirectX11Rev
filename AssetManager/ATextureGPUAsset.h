#pragma once
#include "IGPUAsset.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"

#include <vector>

class ATextureGPUAsset : public IGPUAsset
{
public:
	ATextureGPUAsset() = default;
	virtual ~ATextureGPUAsset();

public:
	virtual const ID3D11Texture2D* const GetTexture2D() = 0;
	virtual const ID3D11ShaderResourceView* const GetSRV() = 0;

protected:
	Texture2DInstance<SRVOption>* m_textureWithSRV = nullptr;
};

