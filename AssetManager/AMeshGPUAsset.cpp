#include "pch.h"
#include "AMeshGPUAsset.h"

using namespace std;

AMeshGPUAsset::~AMeshGPUAsset()
{
	ResetGPUAsset();
}

void AMeshGPUAsset::ResetGPUAsset()
{
	for (ConstantBuffer* vertexBuffer : m_vertexBuffers)
	{
		delete vertexBuffer;
	}

	if (m_indexBuffer != nullptr)
	{
		delete m_indexBuffer;
	}
}
