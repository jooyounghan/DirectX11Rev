#include "pch.h"
#include "ATextureGPUAsset.h"

ATextureGPUAsset::~ATextureGPUAsset()
{
	if (m_resource != nullptr)
	{
		delete m_resource;
	}
}
