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
	LoadStaticMeshPartsField(staticMeshPartsData);
}

void MeshPartsDataWriter::Visit(SkeletalMeshPartsData* skeletalMeshPartsData)
{
	LoadStaticMeshPartsField(skeletalMeshPartsData);
	LoadSkeletalMeshPartsField(skeletalMeshPartsData);
}

void MeshPartsDataWriter::LoadStaticMeshPartsField(StaticMeshPartsData* meshPartData)
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
			const aiVector3D& normal = m_mesh->mNormals[vertexIdx];
			meshPartData->AddNormal(normal.x, normal.y, normal.z * zMultiplier);
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
