#include "AssetManager.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "GraphicsPipeline.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "MathematicalHelper.h"
#include "StringHelper.h"

#include <filesystem>

using namespace std;
using namespace DirectX;
using namespace filesystem;

AssetManager::AssetManager(ID3D11Device* DeviceIn)
    : DeviceCached(DeviceIn)
{
    PreloadAssets();
}

AssetManager::~AssetManager()
{
}

void AssetManager::LoadAssetFile(const string& AssetPathIn)
{
    FILE* InputAssetFile = nullptr;
    fopen_s(&InputAssetFile, AssetPathIn.c_str(), "rb");

    // Asset Name
    size_t AssetNameSize;
    fread(&AssetNameSize, sizeof(size_t), 1, InputAssetFile);
    string AssetName;
    AssetName.resize(AssetNameSize);
    fread(AssetName.data(), sizeof(char), AssetNameSize, InputAssetFile);

    // Asset Type
    EAssetType AssetType;
    fread(&AssetType, sizeof(AssetType), 1, InputAssetFile);

    shared_ptr<IAssetFile> AssetFile;

    switch (AssetType)
    {
    case EAssetType::None:
        break;
    case EAssetType::StaticMesh:
        AssetFile = make_shared<StaticMeshAsset>(AssetName);
        break;
    case EAssetType::SkeletalMesh:
        AssetFile = make_shared<SkeletalMeshAsset>(AssetName);
        break;
    case EAssetType::Bone:
        AssetFile = make_shared<BoneAsset>(AssetName);
        break;
    case EAssetType::Texture:
        break;
    case EAssetType::Animation:
        break;
    default:
        break;
    }

    AssetFile->Deserialize(InputAssetFile, DeviceCached);
    ManagingAssets.emplace(AssetName, AssetFile);

    fclose(InputAssetFile);
}

void AssetManager::LoadModelFile(const string& FilePathIn)
{
    path FilePath = FilePathIn;
    const string FileName = FilePath.stem().string();
    const string FileExtension = FilePath.extension().string();
    
    if (ManagingAssets.find(FileName) == ManagingAssets.end())
    {
        auto test = FilePath.string();
        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(
            StringHelper::ConvertACPToUTF8(FilePathIn),
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
        );

        if (pScene)
        {
            // Load Mesh
            if (pScene->HasMeshes())
            {
                LoadMesh(FileExtension == ".gltf", FileName, pScene);
            }

            if (pScene->HasMaterials())
            {
                LoadMaterial(FileName, pScene);
            }

            if (pScene->HasAnimations())
            {
                LoadAnimation(FileName, pScene);
            }
        }
    }
}

void AssetManager::LoadMesh(bool IsGltf, const string AssetName, const aiScene* const Scene)
{
    shared_ptr<IMeshAsset> Mesh = nullptr;
    shared_ptr<IAssetFile> BoneA = nullptr;

    XMMATRIX RootTransform = DirectX::XMMatrixIdentity();
    aiNode* RootNode = Scene->mRootNode;

    if (HasBone(Scene))
    {
        Mesh = make_shared<SkeletalMeshAsset>(AssetName);
        BoneA = make_shared<BoneAsset>(AssetName);

        SkeletalMeshAsset* SkeletalMeshAPtr = (SkeletalMeshAsset*)Mesh.get();
        BoneAsset* BoneAPtr = (BoneAsset*)BoneA.get();
        ProcessNodeForMesh(IsGltf, Scene, RootNode, SkeletalMeshAPtr, BoneAPtr, RootTransform);

        LoadBone(Scene, SkeletalMeshAPtr, BoneAPtr);

        // Serialize Bone Asset
        BoneA->Serialize();
    }
    else
    {
        Mesh = make_shared<StaticMeshAsset>(AssetName);
        ProcessNodeForMesh(IsGltf, Scene, RootNode, (StaticMeshAsset*)Mesh.get(), RootTransform);
    }

    // Serialize Mesh Asset
    Mesh->Initialize(DeviceCached);

    Mesh->Serialize();
    ManagingAssets.emplace(Mesh->GetAssetName(), Mesh);
}

void AssetManager::LoadMaterial(const string AssetName, const aiScene* const Scene)
{

}

void AssetManager::LoadAnimation(const string AssetName, const aiScene* const Scene)
{
}

bool AssetManager::HasBone(const aiScene* const Scene)
{
    for (UINT MeshIdx = 0; MeshIdx < Scene->mNumMeshes; ++MeshIdx)
    {
        const aiMesh* CurrentMesh = Scene->mMeshes[MeshIdx];
        if (CurrentMesh->HasBones()) return true;
    }
    return false;
}

IAssetFile* AssetManager::GetAsset(const std::string AssetName)
{
    if (ManagingAssets.find(AssetName) != ManagingAssets.end())
    {
        return ManagingAssets[AssetName].get(); 
    }
    else
    {
        return nullptr;
    }
}

void AssetManager::ProcessNodeForMesh(
    bool IsGltf,
    const aiScene* const Scene,
    const aiNode* const Node,
    StaticMeshAsset* StaticMesh, 
    const DirectX::XMMATRIX& ParentMatrix
)
{
    DirectX::XMMATRIX CurrentMatrix(&Node->mTransformation.a1);
    CurrentMatrix = DirectX::XMMatrixTranspose(CurrentMatrix) * ParentMatrix;

    for (UINT MeshIdx = 0; MeshIdx < Node->mNumMeshes; ++MeshIdx)
    {
        const aiMesh* const CurrentMesh = Scene->mMeshes[Node->mMeshes[MeshIdx]];
        LoadMeshElement(IsGltf, CurrentMesh, StaticMesh, CurrentMatrix);
    }

    for (size_t ChildIdx = 0; ChildIdx < Node->mNumChildren; ++ChildIdx)
    {
        ProcessNodeForMesh(IsGltf, Scene, Node->mChildren[ChildIdx], StaticMesh, CurrentMatrix);
    }
}

void AssetManager::ProcessNodeForMesh(
    bool IsGltf,
    const aiScene* const Scene,
    const aiNode* const Node,
    SkeletalMeshAsset* SkeletalMesh,
    BoneAsset* BoneA,
    const DirectX::XMMATRIX& ParentMatrix
)
{
    DirectX::XMMATRIX CurrentMatrix(&Node->mTransformation.a1);
    CurrentMatrix = DirectX::XMMatrixTranspose(CurrentMatrix) * ParentMatrix;

    for (size_t MeshIdx = 0; MeshIdx < Node->mNumMeshes; ++MeshIdx)
    {
        const aiMesh* const CurrentMesh = Scene->mMeshes[Node->mMeshes[MeshIdx]];
        LoadMeshElement(IsGltf, CurrentMesh, SkeletalMesh, BoneA, CurrentMatrix);
    }

    for (size_t ChildIdx = 0; ChildIdx < Node->mNumChildren; ++ChildIdx)
    {
        ProcessNodeForMesh(IsGltf, Scene, Node->mChildren[ChildIdx], SkeletalMesh, BoneA, CurrentMatrix);
    }
}

void AssetManager::ProcessNodeForBone(
    const aiScene* const Scene, 
    const aiNode* const Node, 
    BoneAsset* BoneA
)
{
    bool IsNodeValid = false;
    const string NodeName = Node->mName.C_Str();
    if (Bone* FoundBone = BoneA->FindBone(NodeName))
    {
        IsNodeValid = true;
        BoneA->TraverseDownBone(FoundBone);
    }

    for (size_t ChildIdx = 0; ChildIdx < Node->mNumChildren; ++ChildIdx)
    {
        ProcessNodeForBone(Scene, Node->mChildren[ChildIdx], BoneA);
    }

    if (IsNodeValid) BoneA->TraverseUpBone();
}

void AssetManager::LoadMeshElement(
    bool IsGltf,
    const aiMesh* const Mesh,
    StaticMeshAsset* StaticMesh, 
    const DirectX::XMMATRIX& ParentMatrix
)
{
    size_t VertexStartIdx = StaticMesh->Vertices.size();
    size_t IndicesStartIdx = StaticMesh->Indices.size();

    vector<StaticVertex> TempVertices;
    TempVertices.resize(Mesh->mNumVertices);
    StaticMesh->Vertices.insert(StaticMesh->Vertices.end(), TempVertices.begin(), TempVertices.end());

    // Load Position
    LoadPosition(Mesh, VertexStartIdx, StaticMesh, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, StaticMesh);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, StaticMesh) : LoadTBN(Mesh, VertexStartIdx, StaticMesh);
 
    // Load Index;
    LoadIndices(StaticMesh->Indices, Mesh);

    CalculateTB(Mesh, IndicesStartIdx, StaticMesh);
}

void AssetManager::LoadMeshElement(
    bool IsGltf,
    const aiMesh* const Mesh,
    SkeletalMeshAsset* SkeletalMesh,
    BoneAsset* BoneA,
    const DirectX::XMMATRIX& ParentMatrix
)
{
    size_t VertexStartIdx = SkeletalMesh->Vertices.size();
    size_t IndicesStartIdx = SkeletalMesh->Indices.size();

    vector<SkeletalVertex> TempVertices(Mesh->mNumVertices, SkeletalVertex());
    SkeletalMesh->Vertices.insert(SkeletalMesh->Vertices.end(), TempVertices.begin(), TempVertices.end());

    // Load Position
    LoadPosition(Mesh, VertexStartIdx, SkeletalMesh, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, SkeletalMesh);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, SkeletalMesh) : LoadTBN(Mesh, VertexStartIdx, SkeletalMesh);
    
    // Load Index;
    LoadIndices(SkeletalMesh->Indices, Mesh);

    CalculateTB(Mesh, IndicesStartIdx, SkeletalMesh);
}


template<typename T>
void AssetManager::LoadPosition(
    const aiMesh* const Mesh,
    size_t VertexStartIdx,
    T* MeshAsset,
    const DirectX::XMMATRIX& ParentMatrix
)
{
    if (Mesh->HasPositions())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D CurrentVertex = Mesh->mVertices[VertexIdx];
            DirectX::XMVECTOR TransformedVertex = DirectX::XMVectorSet(CurrentVertex.x, CurrentVertex.y, CurrentVertex.z, 0.f);
            TransformedVertex = DirectX::XMVector3TransformCoord(TransformedVertex, ParentMatrix);
            memcpy(&MeshAsset->Vertices[AccessIdx].Position, &TransformedVertex, sizeof(aiVector3D));
        }
    }
}

void AssetManager::LoadBone(
    const aiScene* const Scene,
    SkeletalMeshAsset* SkeletalMesh,
    BoneAsset* BoneAsset
)
{
    if (Scene->HasMeshes())
    {
        aiMesh* RefMesh = Scene->mMeshes[0];
        if (RefMesh->HasBones())
        {
            // Load Bone Table And Weight
            for (UINT BoneIdx = 0; BoneIdx < RefMesh->mNumBones; ++BoneIdx)
            {
                aiBone* CurrentBone = RefMesh->mBones[BoneIdx];
                XMFLOAT4X4 TempMatrix;
                memcpy(&TempMatrix, &CurrentBone->mOffsetMatrix, sizeof(XMFLOAT4X4));
                BoneAsset->AddBone(CurrentBone->mName.C_Str(), BoneIdx, DirectX::XMLoadFloat4x4(&TempMatrix));

                for (UINT WeightIdx = 0; WeightIdx < CurrentBone->mNumWeights; ++WeightIdx)
                {
                    aiVertexWeight& VertexWeight = CurrentBone->mWeights[WeightIdx];
                    SkeletalVertex& CurrentSkeltalVertex = SkeletalMesh->Vertices[VertexWeight.mVertexId];
                    int InsertedIndex = CurrentSkeltalVertex.BlendWeight.InsertToEmpty(VertexWeight.mWeight);
                    if (InsertedIndex >= 0)
                    {
                        CurrentSkeltalVertex.BlendIndex.InsertWithIndex(InsertedIndex, BoneIdx);
                        bool test = true;
                    }
                }
            }

            // Load Bone Hierachy
            ProcessNodeForBone(Scene, Scene->mRootNode, BoneAsset);
        }
    }
}

template<typename T>
void AssetManager::LoadTextureCoord(
    const aiMesh* const Mesh, 
    size_t VertexStartIdx, 
    T* MeshAsset
)
{
    if (Mesh->HasTextureCoords(0))
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTextureCoord = Mesh->mTextureCoords[0][VertexIdx];
            memcpy(&MeshAsset->Vertices[AccessIdx].TextureCoord, &CurrentTextureCoord, sizeof(aiVector2D));
        }
    }
}

void AssetManager::LoadIndices(
    std::vector<uint32_t>& IndicesIn,
    const aiMesh* const Mesh
)
{
    if (Mesh->HasFaces())
    {
        for (size_t FaceIdx = 0; FaceIdx < Mesh->mNumFaces; ++FaceIdx)
        {
            aiFace& CurrentFace = Mesh->mFaces[FaceIdx];
            for (size_t Index = 0; Index < CurrentFace.mNumIndices; ++Index)
            {
                IndicesIn.emplace_back(CurrentFace.mIndices[Index]);
            }
        }
    }
}

void AssetManager::PreloadAssets()
{
    path AssetPath = path(AssetOutPath);
    path MapPath = path(AssetMapOutPath);
    
    if (!exists(AssetPath) && create_directories(AssetPath)) {/* Do Nothing But Make Directory */ };
    if (!exists(MapPath) && create_directories(MapPath)) {/* Do Nothing But Make Directory */ };

    for (const auto& entry : directory_iterator(AssetPath))
    {
        if (entry.is_regular_file())
        {
            const path filePath = entry.path();
            if (filePath.extension() == AssetExtension)
            {
                LoadAssetFile(filePath.string());
            }
        }
    }
}

template<typename T>
void AssetManager::LoadTBN(
    const aiMesh* const Mesh, 
    size_t VertexStartIdx, 
    T* MeshAsset
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            memcpy(&MeshAsset->Vertices[AccessIdx].Normal, &CurrentNormal, sizeof(aiVector3D));
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];
            memcpy(&MeshAsset->Vertices[AccessIdx].Tangent, &CurrentTangent, sizeof(aiVector3D));
            memcpy(&MeshAsset->Vertices[AccessIdx].Bitangent, &CurrentBiTangent, sizeof(aiVector3D));
        }
    }
}

template<typename T>
void AssetManager::LoadTBNAsGltf(
    const aiMesh* const Mesh,
    size_t VertexStartIdx, 
    T* MeshAsset
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            MeshAsset->Vertices[AccessIdx].Normal.x = CurrentNormal.x;
            MeshAsset->Vertices[AccessIdx].Normal.y = -CurrentNormal.z;
            MeshAsset->Vertices[AccessIdx].Normal.z = CurrentNormal.x;
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];

            MeshAsset->Vertices[AccessIdx].Tangent.x = CurrentTangent.x;
            MeshAsset->Vertices[AccessIdx].Tangent.y = -CurrentTangent.z;
            MeshAsset->Vertices[AccessIdx].Tangent.z = CurrentTangent.x;
            MeshAsset->Vertices[AccessIdx].Bitangent.x = CurrentBiTangent.x;
            MeshAsset->Vertices[AccessIdx].Bitangent.y = -CurrentBiTangent.z;
            MeshAsset->Vertices[AccessIdx].Bitangent.z = CurrentBiTangent.x;
        }
    }
}

template<typename T>
void AssetManager::CalculateTB(const aiMesh* const Mesh, size_t IndexStartIdx, T* MeshAsset)
{
    if (Mesh->HasTangentsAndBitangents())
    {
        // Use Tangent And Bitangent of File
    }
    else
    {
        for (size_t IndexIdx = IndexStartIdx; IndexIdx < MeshAsset->Indices.size(); IndexIdx += 3)
        {
            auto& v0 = MeshAsset->Vertices[MeshAsset->Indices[IndexIdx]];
            auto& v1 = MeshAsset->Vertices[MeshAsset->Indices[IndexIdx + 1]];
            auto& v2 = MeshAsset->Vertices[MeshAsset->Indices[IndexIdx + 2]];

            MathematicalHelper::GetTangentBitangent(
                v0.Position,
                v1.Position,
                v2.Position,
                v0.TextureCoord,
                v1.TextureCoord,
                v2.TextureCoord,
                v0.Normal,
                v1.Normal,
                v2.Normal,
                v0.Tangent,
                v1.Tangent,
                v2.Tangent,
                v0.Bitangent,
                v1.Bitangent,
                v2.Bitangent
            );
        }

    }
}