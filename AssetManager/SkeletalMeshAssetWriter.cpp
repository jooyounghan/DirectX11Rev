#include "pch.h"
#include "SkeletalMeshAssetWriter.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

void SkeletalMeshAssetWriter::LoadMeshPartData(
	MeshPartsData* meshPartData, 
	const bool& isGltf, 
    const aiMesh* const mesh,
	const XMMATRIX& transformation
)
{
	StaticMeshAssetWriter::LoadMeshPartData(meshPartData, isGltf, mesh, transformation);
	SkeletalMeshPartData* skeletalMeshPartData = dynamic_cast<SkeletalMeshPartData*>(meshPartData);

	if (skeletalMeshPartData != nullptr)
	{
        const uint32_t verticesCount = static_cast<uint32_t>(skeletalMeshPartData->GetPositions().size());
        skeletalMeshPartData->ResizeBlendProperties(verticesCount);

        const vector<uint32_t> vertexOffsets = skeletalMeshPartData->GetVertexOffsets();
        const uint32_t vertexOffset = vertexOffsets.empty() ? 0 : vertexOffsets[vertexOffsets.size() - 1];
        
        for (uint32_t boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
        {
            aiBone* currentBone = mesh->mBones[boneIdx];
            for (uint32_t weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx)
            {
                aiVertexWeight& vertexWeight = currentBone->mWeights[weightIdx];

                skeletalMeshPartData->SetBlendProperties(
                    vertexOffset + vertexWeight.mVertexId, 
                    boneIdx, 
                    vertexWeight.mWeight
                );
            }
        }
	}
}
