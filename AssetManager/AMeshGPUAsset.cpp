
#include "AMeshGPUAsset.h"
#include "ConstantBuffer.h"

using namespace std;

AMeshGPUAsset::~AMeshGPUAsset()
{
	ResetGPUAsset();
}

std::vector<ID3D11Buffer*> AMeshGPUAsset::GetVertexBuffers()
{
	vector<ID3D11Buffer*> result;
	for (ConstantBuffer* buffer : m_vertexBuffers)
	{
		result.emplace_back(buffer->GetBuffer());
	}
	return result;
}

ID3D11Buffer* AMeshGPUAsset::GetIndexBuffer()
{
	return m_indexBuffer != nullptr ? m_indexBuffer->GetBuffer() : nullptr;
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
