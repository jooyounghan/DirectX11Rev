#pragma once
#include "IGPUAsset.h"
#include <vector>

template<typename ...IsTextureOption>
class Texture2DInstance;
class SRVOption;
class RTVOption;

class ATextureGPUAsset : public IGPUAsset
{
public:
	ATextureGPUAsset() = default;
	virtual ~ATextureGPUAsset();

public:
	ID3D11Texture2D* GetTexture2D() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11RenderTargetView* GetRTV() const;

protected:
	Texture2DInstance<SRVOption, RTVOption>* m_resource = nullptr;
};

