#include "AMeshData.h"
#include "ConstantBuffer.h"

#include <algorithm>
#include <iterator>

using namespace std;
using namespace DirectX;

XMFLOAT3 AMeshData::EmptyFloat3 = XMFLOAT3(0.f, 0.f, 0.f);
XMFLOAT2 AMeshData::EmptyFloat2 = XMFLOAT2(0.f, 0.f);

AMeshData::~AMeshData()
{
	ResetMeshData();
}

vector<ID3D11Buffer*> AMeshData::GetVertexBuffers() const
{
	const vector<ConstantBuffer*> vertexBuffers = GetVertexConstantBuffers();
	return GetBuffersFromVertexConstantBuffers(vertexBuffers);
}

vector<ID3D11Buffer*> AMeshData::GetVertexBuffersForDepthTest() const
{
	const vector<ConstantBuffer*> vertexBuffers = GetVertexConstantBuffersForDepthTest();
	return GetBuffersFromVertexConstantBuffers(vertexBuffers);
}

void AMeshData::SetVertexBuffer(ConstantBuffer* vertexBuffer)
{
	if (m_vertexBuffer) delete m_vertexBuffer;
	m_vertexBuffer = vertexBuffer; 
}

void AMeshData::SetUVBuffer(ConstantBuffer* uvBuffer) 
{
	if (m_uvBuffer) delete m_uvBuffer;
	m_uvBuffer = uvBuffer;
}

void AMeshData::SetIndexBuffer(ConstantBuffer* indexBuffer)
{ 
	if (m_indexBuffer) delete m_indexBuffer; 
	m_indexBuffer = indexBuffer; 
}

const XMFLOAT3& AMeshData::GetPosition(const size_t& index) const
{
	if (m_positions.size() > index)
	{
		return m_positions[index];
	}
	return EmptyFloat3;
}

const XMFLOAT2& AMeshData::GetUVTextureCoord(const size_t& index) const
{
	if (m_uvTextures.size() > index)
	{
		return m_uvTextures[index];
	}
	return EmptyFloat2;
}

void AMeshData::AddPosition(const float& x, const float& y, const float& z)
{
	m_positions.emplace_back(XMFLOAT3(x, y, z));
}

void AMeshData::AddUVTexture(const float& u, const float& v)
{
	m_uvTextures.emplace_back(XMFLOAT2(u, v));
}

void AMeshData::ResetMeshData()
{
	if (m_vertexBuffer) delete m_vertexBuffer;
	if (m_uvBuffer) delete m_uvBuffer;
	if (m_indexBuffer) delete m_indexBuffer;
}

vector<ID3D11Buffer*> AMeshData::GetBuffersFromVertexConstantBuffers(const vector<ConstantBuffer*>& vertexConstantBuffers) const
{
	vector<ID3D11Buffer*> buffers;
	buffers.reserve(vertexConstantBuffers.size());

	transform(vertexConstantBuffers.begin(), vertexConstantBuffers.end(),
		back_inserter(buffers),
		[](const ConstantBuffer* constantBuffer) -> ID3D11Buffer* 
		{
			return constantBuffer->GetBuffer();
		}
	);
	return buffers;
}
