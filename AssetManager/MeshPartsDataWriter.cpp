#include "MeshPartsDataWriter.h"

#include "StaticMeshPartsData.h"
#include "SkeletalMeshPartsData.h"

#include "MathematicalHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace std;
using namespace DirectX;

MeshPartsDataWriter::MeshPartsDataWriter(const bool& isGltf, const aiMesh* const mesh, const DirectX::XMMATRIX& transformation)
	: m_isGltf(isGltf), m_mesh(mesh), m_transformation(transformation)
{
}

void MeshPartsDataWriter::Visit(StaticMeshPartsData* staticMeshPartsData)
{
	LoadMeshPartsField(staticMeshPartsData);
	LoadStaticMeshPartsField(staticMeshPartsData);
}

void MeshPartsDataWriter::Visit(SkeletalMeshPartsData* skeletalMeshPartsData)
{
	LoadMeshPartsField(skeletalMeshPartsData);
	LoadStaticMeshPartsField(skeletalMeshPartsData);
	LoadSkeletalMeshPartsField(skeletalMeshPartsData);
}

void MeshPartsDataWriter::LoadMeshPartsField(MeshPartsData* meshPartData)
{
	if (m_mesh->HasFaces())
	{
		const uint32_t vertexOffset = static_cast<uint32_t>(meshPartData->GetPositions().size());
		const uint32_t indexOffset = static_cast<uint32_t>(meshPartData->GetIndices().size());

		meshPartData->AddPartOffsets(vertexOffset, indexOffset);
		for (uint32_t faceIdx = 0; faceIdx < m_mesh->mNumFaces; ++faceIdx)
		{
			aiFace& currentFace = m_mesh->mFaces[faceIdx];

			for (uint32_t idx = 0; idx < currentFace.mNumIndices; ++idx)
			{
				meshPartData->AddIndex(vertexOffset, static_cast<uint32_t>(currentFace.mIndices[idx]));
			}
		}
	}

	if (m_mesh->HasPositions())
	{
		for (uint32_t vertexIdx = 0; vertexIdx < m_mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& vertex = m_mesh->mVertices[vertexIdx];
			DirectX::XMVECTOR transformedVertex = DirectX::XMVectorSet(vertex.x, vertex.y, vertex.z, 0.f);
			transformedVertex = DirectX::XMVector3TransformCoord(transformedVertex, m_transformation);
			meshPartData->AddPosition(transformedVertex.m128_f32[0], transformedVertex.m128_f32[1], transformedVertex.m128_f32[2]);
		}
	}

	if (m_mesh->HasTextureCoords(0))
	{
		for (size_t vertexIdx = 0; vertexIdx < m_mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& uvCoordinate = m_mesh->mTextureCoords[0][vertexIdx];
			meshPartData->AddUVTexture(uvCoordinate.x, uvCoordinate.y);
		}
	}

	if (m_mesh->HasNormals())
	{
		const float zMultiplier = m_isGltf ? -1.0f : 1.0f;
		for (uint32_t vertexIdx = 0; vertexIdx < m_mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& normal = m_mesh->mVertices[vertexIdx];
			meshPartData->AddNormal(normal.x, normal.y, normal.z * zMultiplier);
		}
	}
}

void MeshPartsDataWriter::LoadStaticMeshPartsField(StaticMeshPartsData* staticMeshPartsData)
{
	if (m_mesh->HasTangentsAndBitangents())
	{
		for (size_t vertexIdx = 0; vertexIdx < m_mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& tangent = m_mesh->mTangents[vertexIdx];
			staticMeshPartsData->AddTangent(tangent.x, tangent.y, tangent.z);
		}
	}
	else
	{
		const vector<uint32_t>& indices = staticMeshPartsData->GetIndices();
		const vector<uint32_t> indexOffsets = staticMeshPartsData->GetIndexOffsets();
		const uint32_t indexOffset = indexOffsets.empty() ? 0 : indexOffsets[indexOffsets.size() - 1];


		for (size_t idx = indexOffset; idx < indices.size(); idx += 3)
		{
			const uint32_t& index0 = indices[idx];
			const uint32_t& index1 = indices[idx + 1];
			const uint32_t& index2 = indices[idx + 2];

			const XMFLOAT3& p0 = staticMeshPartsData->GetPosition(index0);
			const XMFLOAT3& p1 = staticMeshPartsData->GetPosition(index1);
			const XMFLOAT3& p2 = staticMeshPartsData->GetPosition(index2);

			const XMFLOAT2& uv0 = staticMeshPartsData->GetUVTextureCoord(index0);
			const XMFLOAT2& uv1 = staticMeshPartsData->GetUVTextureCoord(index1);
			const XMFLOAT2& uv2 = staticMeshPartsData->GetUVTextureCoord(index2);

			const XMFLOAT3& n0 = staticMeshPartsData->GetNormal(index0);
			const XMFLOAT3& n1 = staticMeshPartsData->GetNormal(index1);
			const XMFLOAT3& n2 = staticMeshPartsData->GetNormal(index2);

			XMFLOAT3 t0, t1, t2;
			XMFLOAT3 b0, b1, b2;

			MathematicalHelper::GetTangentBitangent(
				p0, p1, p2,
				uv0, uv1, uv2,
				n0, n1, n2,
				t0, t1, t2,
				b0, b1, b2
			);

			staticMeshPartsData->AddTangent(t0.x, t0.y, t0.z);
			staticMeshPartsData->AddTangent(t1.x, t1.y, t1.z);
			staticMeshPartsData->AddTangent(t2.x, t2.y, t2.z);
		}
	}

}

void MeshPartsDataWriter::LoadSkeletalMeshPartsField(SkeletalMeshPartsData* skeletalMeshPartsData)
{
	const uint32_t verticesCount = static_cast<uint32_t>(skeletalMeshPartsData->GetPositions().size());
	skeletalMeshPartsData->ResizeBlendProperties(verticesCount);

	const vector<uint32_t> vertexOffsets = skeletalMeshPartsData->GetVertexPartOffsets();
	const uint32_t vertexOffset = vertexOffsets.empty() ? 0 : vertexOffsets[vertexOffsets.size() - 1];

	for (uint32_t boneIdx = 0; boneIdx < m_mesh->mNumBones; ++boneIdx)
	{
		aiBone* currentBone = m_mesh->mBones[boneIdx];
		for (uint32_t weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx)
		{
			aiVertexWeight& vertexWeight = currentBone->mWeights[weightIdx];

			skeletalMeshPartsData->SetBlendProperties(
				vertexOffset + vertexWeight.mVertexId,
				boneIdx,
				vertexWeight.mWeight
			);
		}
	}
}
