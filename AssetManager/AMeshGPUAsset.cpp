#include "AMeshGPUAsset.h"
#include "ConstantBuffer.h"

using namespace std;

AMeshGPUAsset::~AMeshGPUAsset()
{
	ResetGPUAsset();
}

vector<ID3D11Buffer*> AMeshGPUAsset::GetD3D11VertexBuffers() const
{
	vector<ID3D11Buffer*> result;
	for (ConstantBuffer* buffer : m_vertexBuffers)
	{
		result.emplace_back(buffer->GetBuffer());
	}
	return result;
}

ID3D11Buffer* AMeshGPUAsset::GetD3D11IndexBuffer() const
{
	return m_indexBuffer != nullptr ? m_indexBuffer->GetBuffer() : nullptr;
}

void AMeshGPUAsset::SetIndexBuffer(ConstantBuffer* indexBuffer) 
{ 
	if (m_indexBuffer) delete m_indexBuffer; 
	m_indexBuffer = indexBuffer; 
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
