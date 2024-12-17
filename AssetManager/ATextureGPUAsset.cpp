#include "pch.h"
#include "ATextureGPUAsset.h"

ATextureGPUAsset::~ATextureGPUAsset()
{
	if (m_textureWithSRV != nullptr)
	{
		delete m_textureWithSRV;
	}
}
