#include "SkeletalMeshPartsData.h"
#include "ConstantBuffer.h"

using namespace std;
using namespace DirectX;


void SkeletalMeshPartsData::ResizeBlendProperties(const size_t& resizeCount)
{
	m_blendWeight.resize(resizeCount);
	m_blendIndex.resize(resizeCount, XMINT4(-1, -1, -1, -1));
}

void SkeletalMeshPartsData::SetBlendProperties(
	const size_t& vertexIndex,
	const int& boneIndex,
	const float& blendWeight
)
{
	if (m_blendWeight.size() <= vertexIndex) return;
	if (m_blendIndex.size() <= vertexIndex) return;

	XMFLOAT4& CurrentBlendWeight = m_blendWeight[vertexIndex];
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

	SerializeHelper::SerializeVectorContainer(m_blendWeight, fileIn);
	SerializeHelper::SerializeVectorContainer(m_blendIndex, fileIn);
}

void SkeletalMeshPartsData::Deserialize(FILE* fileIn)
{
	StaticMeshPartsData::Deserialize(fileIn);

	m_blendWeight = DeserializeHelper::DeserializeVectorContainer<vector<XMFLOAT4>>(fileIn);
	m_blendIndex = DeserializeHelper::DeserializeVectorContainer<vector<XMINT4>>(fileIn);
}


std::vector<UINT> SkeletalMeshPartsData::GetStrides()
{
	return vector<UINT> {
		sizeof(XMFLOAT3),
			sizeof(XMFLOAT2),
			sizeof(XMFLOAT3),
			sizeof(XMFLOAT3),
			sizeof(XMFLOAT4),
			sizeof(XMINT4)
	};
}

std::vector<UINT> SkeletalMeshPartsData::GetOffsets()
{
	return vector<UINT>{ 0, 0, 0, 0, 0, 0 };
}

void SkeletalMeshPartsData::InitializeGPUAsset(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	StaticMeshPartsData::InitializeGPUAsset(device, deviceContext);
	ConstantBuffer* blendWeightBuffer = new ConstantBuffer(sizeof(XMFLOAT4), static_cast<UINT>(m_blendWeight.size()), m_blendWeight.data(), D3D11_BIND_VERTEX_BUFFER);
	ConstantBuffer* blendIndexBuffer = new ConstantBuffer(sizeof(XMINT4), static_cast<UINT>(m_blendIndex.size()), m_blendIndex.data(), D3D11_BIND_VERTEX_BUFFER);

	blendWeightBuffer->Initialize(device);
	blendIndexBuffer->Initialize(device);

	m_vertexBuffers.emplace_back(blendWeightBuffer);
	m_vertexBuffers.emplace_back(blendIndexBuffer);
}

void SkeletalMeshPartsData::Accept(IMeshPartsDataVisitor& visitor)
{
	visitor.Visit(this);
}
