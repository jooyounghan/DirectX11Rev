
#include "ATextureGPUAsset.h"
#include "Texture2DInstance.h"
#include "SRVOption.h"
#include "RTVOption.h"

ATextureGPUAsset::~ATextureGPUAsset()
{
	if (m_resource != nullptr)
	{
		delete m_resource;
	}
}

ID3D11Texture2D* ATextureGPUAsset::GetTexture2D() const { return m_resource->GetTexture2D(); }
ID3D11ShaderResourceView* ATextureGPUAsset::GetSRV() const { return m_resource->GetSRV(); }
ID3D11RenderTargetView* ATextureGPUAsset::GetRTV() const { return m_resource->GetRTV(); }
