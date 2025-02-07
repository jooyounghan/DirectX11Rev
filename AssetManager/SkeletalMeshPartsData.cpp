#include "SkeletalMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;


void SkeletalMeshPartsData::SetBlendWeightBuffer(ConstantBuffer* blendWeightBuffer)
{
	if (m_blendWeightBuffer) delete m_blendWeightBuffer;
	m_blendWeightBuffer = blendWeightBuffer;
}

void SkeletalMeshPartsData::SetBlendIndexBuffer(ConstantBuffer* blendIndexBuffer)
{
	if (m_blendIndexBuffer) delete m_blendIndexBuffer;
	m_blendIndexBuffer = blendIndexBuffer;
}

void SkeletalMeshPartsData::ResizeBlendProperties(const size_t& resizeCount)
{
	m_blendWeights.resize(resizeCount);
	m_blendIndex.resize(resizeCount, XMINT4(-1, -1, -1, -1));
}

void SkeletalMeshPartsData::SetBlendProperties(
	const size_t& vertexIndex,
	const int& boneIndex,
	const float& blendWeight
)
{
	if (m_blendWeights.size() <= vertexIndex) return;
	if (m_blendIndex.size() <= vertexIndex) return;

	XMFLOAT4& CurrentBlendWeight = m_blendWeights[vertexIndex];
	XMINT4& CurrentBlendIndex = m_blendIndex[vertexIndex];

	if (CurrentBlendIndex.x == -1)
	{
		CurrentBlendIndex.x = boneIndex; CurrentBlendWeight.x = blendWeight;
	}
	else if (CurrentBlendIndex.y == -1)
	{
		CurrentBlendIndex.y = boneIndex; CurrentBlendWeight.y = blendWeight;
	}
	else if (CurrentBlendIndex.z == -1)
	{
		CurrentBlendIndex.z = boneIndex; CurrentBlendWeight.z = blendWeight;
	}
	else if (CurrentBlendIndex.w == -1)
	{
		CurrentBlendIndex.w = boneIndex; CurrentBlendWeight.w = blendWeight;
	}
	else
	{

	}
}

void SkeletalMeshPartsData::Serialize(FILE* fileIn) const
{
	StaticMeshPartsData::Serialize(fileIn);

	SerializeHelper::SerializeVectorContainer(m_blendWeights, fileIn);
	SerializeHelper::SerializeVectorContainer(m_blendIndex, fileIn);
}

void SkeletalMeshPartsData::Deserialize(FILE* fileIn)
{
	StaticMeshPartsData::Deserialize(fileIn);

	m_blendWeights = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT4>>(fileIn);
	m_blendIndex = DeserializeHelper::DeserializeVectorContainer<vector<XMINT4>>(fileIn);
}

std::vector<ConstantBuffer*> SkeletalMeshPartsData::GetVertexConstantBuffers() const
{
	return std::vector<ConstantBuffer*>
	{
		m_vertexBuffer,
		m_uvBuffer,
		m_normalBuffer,
		m_blendWeightBuffer,
		m_blendIndexBuffer
	};
}

std::vector<ConstantBuffer*> SkeletalMeshPartsData::GetVertexConstantBuffersForDepthTest() const
{
	return std::vector<ConstantBuffer*> { m_vertexBuffer, m_blendWeightBuffer, m_blendIndexBuffer };
}


std::vector<UINT> SkeletalMeshPartsData::GetStrides() const
{
	return vector<UINT> 
	{
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT2),
		sizeof(XMFLOAT3),
		sizeof(XMFLOAT4),
		sizeof(XMINT4)
	};
}

std::vector<UINT> SkeletalMeshPartsData::GetOffsets() const
{
	return vector<UINT>{ 0, 0, 0, 0, 0 };
}

std::vector<UINT> SkeletalMeshPartsData::GetStridesForDepthTest() const
{
	return vector<UINT> { sizeof(XMFLOAT3), sizeof(XMFLOAT4), sizeof(XMINT4) };
}

std::vector<UINT> SkeletalMeshPartsData::GetOffsetsForDepthTest() const
{
	return vector<UINT>{ 0, 0, 0 };
}

void SkeletalMeshPartsData::Accept(IMeshPartsDataVisitor& visitor)
{
	visitor.Visit(this);
}

void SkeletalMeshPartsData::ResetMeshData()
{
	StaticMeshPartsData::ResetMeshData();
	if (m_blendWeightBuffer) delete m_blendWeightBuffer;
	if (m_blendIndexBuffer) delete m_blendIndexBuffer;
}
