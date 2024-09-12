#include "AssetManager.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include "GraphicsPipeline.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "MapAsset.h"

#include "BaseCubeMeshAsset.h"
#include "BaseSphereMeshAsset.h"

#include "NormalTextureAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "MathematicalHelper.h"
#include "StringHelper.h"

#include <filesystem>

#include "DirectXTexEXR.h"

using namespace std;
using namespace DirectX;
using namespace filesystem;

unordered_map<string, EFileType> AssetManager::FileExtensionToType
{
    { ".gltf", EFileType::ModelFile },
    { ".fbx", EFileType::ModelFile },
    { ".obj", EFileType::ModelFile },
    { ".jpg", EFileType::NormalTextureFile },
    { ".jpeg", EFileType::NormalTextureFile},
    { ".png", EFileType::NormalTextureFile },
    { ".exr", EFileType::EXRTextureFile },
    { ".dds", EFileType::DDSTextureFile },
};

AssetManager::AssetManager()
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

    if (InputAssetFile != nullptr)
    {
        // Asset Name
        size_t AssetNameSize;
        fread(&AssetNameSize, sizeof(size_t), 1, InputAssetFile);
        string AssetName;
        AssetName.resize(AssetNameSize);
        fread(AssetName.data(), sizeof(char), AssetNameSize, InputAssetFile);

        // Asset Type
        EAssetType AssetType;
        fread(&AssetType, sizeof(AssetType), 1, InputAssetFile);

        switch (AssetType)
        {
        case EAssetType::None:
            break;
        case EAssetType::StaticMesh:
            LoadAssetFileHelper(InputAssetFile, ManagingStaticMeshes, AssetName, false);
            break;
        case EAssetType::SkeletalMesh:
            LoadAssetFileHelper(InputAssetFile, ManagingSkeletalMeshes, AssetName, false);
            break;
        case EAssetType::Bone:
            LoadAssetFileHelper(InputAssetFile, ManagingBones, AssetName, false);
            break;
        case EAssetType::Map:
            LoadAssetFileHelper(InputAssetFile, ManagingMaps, AssetName, this, false);
            break;
        case EAssetType::NormalTexture:
            LoadAssetFileHelper(InputAssetFile, ManagingNormalTextures, AssetName);
            break;
        case EAssetType::EXRTexture:
            LoadAssetFileHelper(InputAssetFile, ManagingEXRTextures, AssetName);
            break;
        case EAssetType::DDSTexture:
            LoadAssetFileHelper(InputAssetFile, ManagingDDSTextures, AssetName);
            break;
        case EAssetType::Animation:
            break;
        default:
            break;
        }
        fclose(InputAssetFile);
    }
    else
    {

    }
}

template<typename T, typename ...Args>
void AssetManager::LoadAssetFileHelper(
    FILE* FileIn,
    std::unordered_map<std::string, std::shared_ptr<T>>& ManagingContainer, 
    const std::string& AssetName,
    Args... AssetConstructArgs
)
{
    shared_ptr<T> AssetFile = make_shared<T>(AssetName, AssetConstructArgs...);
    AssetFile->Deserialize(FileIn, this);
    ManagingContainer.emplace(AssetName, AssetFile);
}


void AssetManager::LoadFile(const std::string& FilePathIn)
{
    path FilePath = FilePathIn;
    const string FileName = FilePath.stem().string();
    const string FileExtension = FilePath.extension().string();

    if (FileExtensionToType.find(FileExtension) != FileExtensionToType.end())
    {
        switch (FileExtensionToType[FileExtension])
        {
        case EFileType::ModelFile:
            LoadModelFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::NormalTextureFile:
            LoadNormalTextureFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::EXRTextureFile:
            LoadEXRTextureFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::DDSTextureFile:
            LoadDDSTextureFile(FilePathIn, FileName, FileExtension);
            break;
        default:
            break;
        }
    }
}

void AssetManager::LoadModelFile(const string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn)
{
    if (FileNameToAssetNames.find(FileNameIn) == FileNameToAssetNames.end())
    {
        FileNameStack.push(FileNameIn);

        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(
            StringHelper::ConvertACPToUTF8(FilePathIn),
            aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_ConvertToLeftHanded
        );

        if (pScene)
        {
            // Load Mesh
            if (pScene->HasMeshes())
            {
                LoadMesh(FileExtensionIn == ".gltf", FileNameIn, pScene);
            }

            if (pScene->HasMaterials())
            {
                LoadMaterial(FileNameIn, pScene);
            }

            if (pScene->HasAnimations())
            {
                LoadAnimation(FileNameIn, pScene);
            }
        }
        FileNameStack.pop();

        assert(FileNameStack.empty());
    }
}

void AssetManager::LoadNormalTextureFile(
    const string& FilePathIn, 
    const std::string& FileNameIn, 
    const std::string& FileExtensionIn
)
{
    FILE* FileHandle;
    fopen_s(&FileHandle, FilePathIn.c_str(), "rb");

    if (FileHandle != nullptr)
    {
        uint8_t* ImageBuffer = nullptr;
        int WidthOut, HeightOut, ChannelOut;
        ImageBuffer = stbi_load_from_file(FileHandle, &WidthOut, &HeightOut, &ChannelOut, 4);

        if (ImageBuffer != nullptr)
        {
            shared_ptr<NormalTextureAsset> TextureAssetLoaded = make_shared<NormalTextureAsset>(FileNameIn, ImageBuffer, WidthOut, HeightOut);
            AddToManagingContainer(ManagingNormalTextures, TextureAssetLoaded);
        }
        fclose(FileHandle);

    }
    else
    {
    }
}

void AssetManager::LoadEXRTextureFile(
    const std::string& FilePathIn, 
    const std::string& FileNameIn, 
    const std::string& FileExtensionIn
)
{
    ScratchImage scratch;
    TexMetadata metaData;
    HRESULT hResult = LoadFromEXRFile(StringHelper::ConvertACPToWString(FilePathIn).c_str(), &metaData, scratch);
    if (!FAILED(hResult))
    {
        shared_ptr<EXRTextureAsset> TextureAssetLoaded = make_shared<EXRTextureAsset>(FileNameIn, scratch, metaData);
        AddToManagingContainer(ManagingEXRTextures, TextureAssetLoaded);
    }
}

void AssetManager::LoadDDSTextureFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn)
{
    ScratchImage scratch;
    TexMetadata metaData;
    HRESULT hResult = LoadFromDDSFile(StringHelper::ConvertACPToWString(FilePathIn).c_str(), DDS_FLAGS_NONE, &metaData, scratch);
    if (!FAILED(hResult))
    {
        shared_ptr<DDSTextureAsset> TextureAssetLoaded = make_shared<DDSTextureAsset>(FileNameIn, scratch, metaData);
        AddToManagingContainer(ManagingDDSTextures, TextureAssetLoaded);
    }
}

void AssetManager::LoadMesh(bool IsGltf, const string AssetName, const aiScene* const Scene)
{
    XMMATRIX RootTransform = DirectX::XMMatrixIdentity();
    aiNode* RootNode = Scene->mRootNode;

    if (HasBone(Scene))
    {
        shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetLoaded = make_shared<SkeletalMeshAsset>(AssetName, true);
        shared_ptr<BoneAsset> BoneAssetLoaded = make_shared<BoneAsset>(AssetName, true);

        SkeletalMeshAssetLoaded->SetLinkedBoneAsset(BoneAssetLoaded);

        SkeletalMeshAsset* SkeletalMeshAPtr = SkeletalMeshAssetLoaded.get();
        BoneAsset* BoneAPtr = BoneAssetLoaded.get();
        ProcessNodeForMesh(IsGltf, Scene, RootNode, SkeletalMeshAPtr, BoneAPtr, RootTransform);

        LoadBone(Scene, SkeletalMeshAPtr, BoneAPtr);

        // Serialize Bone Asset
        AddToManagingContainer(ManagingBones, BoneAssetLoaded);

        SkeletalMeshAssetLoaded->Initialize();
        AddToManagingContainer(ManagingSkeletalMeshes, SkeletalMeshAssetLoaded);
    }
    else
    {
        shared_ptr<StaticMeshAsset> StaticMeshLoaded = make_shared<StaticMeshAsset>(AssetName, true);
        
        ProcessNodeForMesh(IsGltf, Scene, RootNode, (StaticMeshAsset*)StaticMeshLoaded.get(), RootTransform);

        StaticMeshLoaded->Initialize();
        AddToManagingContainer(ManagingStaticMeshes, StaticMeshLoaded);
    }
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

AAssetFile* AssetManager::GetManagingAsset(const std::string& AssetNameIn)
{
    AAssetFile* Result = nullptr;
    const string AssetSuffix = AssetNameIn.substr(AssetNameIn.find_last_of("_"));
    EAssetType AssetType = AAssetFile::AssetSuffixToType[AssetSuffix];

    switch (AssetType)
    {
    case EAssetType::BaseMesh:
        Result = GetManagingAssetHelper(ManagingBaseMeshes, AssetNameIn);
        break;
    case EAssetType::StaticMesh:
        Result = GetManagingAssetHelper(ManagingStaticMeshes, AssetNameIn).get();
        break;
    case EAssetType::SkeletalMesh:
        Result = GetManagingAssetHelper(ManagingSkeletalMeshes, AssetNameIn).get();
        break;
    case EAssetType::Bone:
        Result = GetManagingAssetHelper(ManagingBones, AssetNameIn).get();
        break;
    case EAssetType::Map:
        Result = GetManagingAssetHelper(ManagingMaps, AssetNameIn).get();
        break;
    case EAssetType::Material:
        break;
    case EAssetType::NormalTexture:
        Result = GetManagingAssetHelper(ManagingNormalTextures, AssetNameIn).get();
        break;
    case EAssetType::EXRTexture:
        Result = GetManagingAssetHelper(ManagingEXRTextures, AssetNameIn).get();
        break;
    case EAssetType::DDSTexture:
        Result = GetManagingAssetHelper(ManagingDDSTextures, AssetNameIn).get();
        break;
    case EAssetType::Animation:
        break;
    default:
        break;
    }

    return Result;
}

std::shared_ptr<MapAsset> AssetManager::GetManagingMap(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingMaps, MapAssetName);
}

std::shared_ptr<BoneAsset> AssetManager::GetManagingBone(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingBones, MapAssetName);
}

std::shared_ptr<StaticMeshAsset> AssetManager::GetManagingStaticMesh(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingStaticMeshes, MapAssetName);
}

std::shared_ptr<SkeletalMeshAsset> AssetManager::GetManagingSkeletalMesh(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingSkeletalMeshes, MapAssetName);
}

template<typename T>
void AssetManager::AddToManagingContainer(
    unordered_map<string, T>& ManagingContainer, 
    T& AddedAsset
)
{
    AddedAsset->Serialize();
    ManagingContainer.emplace(AddedAsset->GetAssetName(), AddedAsset);
}

template<typename T>
T AssetManager::GetManagingAssetHelper(
    std::unordered_map<string, T>& ManagingContainer,
    const string AssetName
)
{
    if (ManagingContainer.find(AssetName) != ManagingContainer.end())
    {
        return ManagingContainer[AssetName];
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

void AssetManager::RestructBaseVertices(const unsigned int& NumVertices, ANBTMeshAsset* MeshAssetIn)
{
    vector<XMFLOAT3> TempPositions;
    vector<XMFLOAT2> TempUVTextures;
    vector<XMFLOAT3> TempNormals;
    vector<XMFLOAT3> TempTangents;
    vector<XMFLOAT3> TempBitangents;

    TempPositions.resize(NumVertices);
    TempUVTextures.resize(NumVertices);
    TempNormals.resize(NumVertices);
    TempTangents.resize(NumVertices);
    TempBitangents.resize(NumVertices);

    MeshAssetIn->Positions.Vertices.insert(MeshAssetIn->Positions.Vertices.end(), TempPositions.begin(), TempPositions.end());
    MeshAssetIn->UVTextures.Vertices.insert(MeshAssetIn->UVTextures.Vertices.end(), TempUVTextures.begin(), TempUVTextures.end());
    MeshAssetIn->Normals.Vertices.insert(MeshAssetIn->Normals.Vertices.end(), TempNormals.begin(), TempNormals.end());
    MeshAssetIn->Tangents.Vertices.insert(MeshAssetIn->Tangents.Vertices.end(), TempTangents.begin(), TempTangents.end());
    MeshAssetIn->Bitangents.Vertices.insert(MeshAssetIn->Bitangents.Vertices.end(), TempBitangents.begin(), TempBitangents.end());
}

void AssetManager::LoadMeshElement(
    bool IsGltf,
    const aiMesh* const Mesh,
    StaticMeshAsset* StaticMesh, 
    const DirectX::XMMATRIX& ParentMatrix
)
{
    size_t VertexStartIdx = StaticMesh->Positions.Vertices.size();
    size_t IndicesStartIdx = StaticMesh->Indices.size();

    RestructBaseVertices(Mesh->mNumVertices, StaticMesh);

    // Load Position
    LoadPosition(Mesh, VertexStartIdx, StaticMesh, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, StaticMesh);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, StaticMesh) : LoadTBN(Mesh, VertexStartIdx, StaticMesh);
 
    // Load Index;
    LoadIndices(StaticMesh->Indices, VertexStartIdx, Mesh);

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
    size_t VertexStartIdx = SkeletalMesh->Positions.Vertices.size();
    size_t IndicesStartIdx = SkeletalMesh->Indices.size();

    RestructBaseVertices(Mesh->mNumVertices, SkeletalMesh);

    vector<XMFLOAT4> TempBlendWeight;
    vector<XMINT4> TempBlendIndex;

    TempBlendWeight.resize(Mesh->mNumVertices);
    TempBlendIndex.resize(Mesh->mNumVertices, XMINT4(-1, -1, -1, -1));

    SkeletalMesh->BlendWeight.Vertices.insert(SkeletalMesh->BlendWeight.Vertices.end(), TempBlendWeight.begin(), TempBlendWeight.end());
    SkeletalMesh->BlendIndex.Vertices.insert(SkeletalMesh->BlendIndex.Vertices.end(), TempBlendIndex.begin(), TempBlendIndex.end());


    // Load Position
    LoadPosition(Mesh, VertexStartIdx, SkeletalMesh, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, SkeletalMesh);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, SkeletalMesh) : LoadTBN(Mesh, VertexStartIdx, SkeletalMesh);
    
    // Load Index;
    LoadIndices(SkeletalMesh->Indices, VertexStartIdx, Mesh);

    CalculateTB(Mesh, IndicesStartIdx, SkeletalMesh);
}



template<typename T>
void AssetManager::LoadPosition(
    const aiMesh* const Mesh,
    size_t VertexStartIdx,
    T* MeshObjectInstance,
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
            memcpy(&MeshObjectInstance->Positions.Vertices[AccessIdx], &TransformedVertex, sizeof(aiVector3D));
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

                    XMFLOAT4& CurrentBlendWeight = SkeletalMesh->BlendWeight.Vertices[VertexWeight.mVertexId];
                    XMINT4& CurrentBlendIndex = SkeletalMesh->BlendIndex.Vertices[VertexWeight.mVertexId];

                    int InsertedIndex = -1;
                    if (CurrentBlendIndex.x == -1)
                    {
                        CurrentBlendIndex.x = BoneIdx; CurrentBlendWeight.x = VertexWeight.mWeight;
                    }
                    else if (CurrentBlendIndex.y == -1)
                    {
                        CurrentBlendIndex.y = BoneIdx; CurrentBlendWeight.y = VertexWeight.mWeight;
                    }
                    else if (CurrentBlendIndex.z == -1)
                    {
                        CurrentBlendIndex.z = BoneIdx; CurrentBlendWeight.z = VertexWeight.mWeight;
                    }
                    else if (CurrentBlendIndex.w == -1)
                    {
                        CurrentBlendIndex.w = BoneIdx; CurrentBlendWeight.w = VertexWeight.mWeight;
                    }
                    else
                    {

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
    T* MeshObjectInstance
)
{
    if (Mesh->HasTextureCoords(0))
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTextureCoord = Mesh->mTextureCoords[0][VertexIdx];
            memcpy(&MeshObjectInstance->UVTextures.Vertices[AccessIdx], &CurrentTextureCoord, sizeof(aiVector2D));
        }
    }
}

void AssetManager::LoadIndices(
    std::vector<uint32_t>& IndicesIn,
    size_t VertexStartIdx,
    const aiMesh* const Mesh
)
{
    if (Mesh->HasFaces())
    {
        for (UINT FaceIdx = 0; FaceIdx < Mesh->mNumFaces; ++FaceIdx)
        {
            aiFace& CurrentFace = Mesh->mFaces[FaceIdx];
            for (UINT Index = 0; Index < CurrentFace.mNumIndices; ++Index)
            {
                IndicesIn.emplace_back(static_cast<uint32_t>(VertexStartIdx + CurrentFace.mIndices[Index]));
            }
        }
    }
}

void AssetManager::PreloadAssets()
{
    vector<BaseMeshAsset*> BaseMeshAssets
    {
        BaseCubeMeshAsset::GetInstance(),
        BaseSphereMeshAsset::GetInstance()
    };

    for (BaseMeshAsset* BaseMeshAssetInstance : BaseMeshAssets)
    {
        AddToManagingContainer(ManagingBaseMeshes, BaseMeshAssetInstance);
    }

    path AssetPath = path(AssetOutPath);
    path MapPath = path(MapAssetOutPath);

    if (!exists(AssetPath) && create_directories(AssetPath)) {/* Do Nothing But Make Directory */ };
    if (!exists(MapPath) && create_directories(MapPath)) {/* Do Nothing But Make Directory */ };

    // TODO : Asset의 로딩 순서 결정하기
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

    for (const auto& entry : directory_iterator(MapPath))
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
    T* MeshObjectInstance
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            memcpy(&MeshObjectInstance->Normals.Vertices[AccessIdx], &CurrentNormal, sizeof(aiVector3D));
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];
            memcpy(&MeshObjectInstance->Tangents.Vertices[AccessIdx], &CurrentTangent, sizeof(aiVector3D));
            memcpy(&MeshObjectInstance->Bitangents.Vertices[AccessIdx], &CurrentBiTangent, sizeof(aiVector3D));
        }
    }
}

template<typename T>
void AssetManager::LoadTBNAsGltf(
    const aiMesh* const Mesh,
    size_t VertexStartIdx, 
    T* MeshObjectInstance
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            MeshObjectInstance->Normals.Vertices[AccessIdx].x = CurrentNormal.x;
            MeshObjectInstance->Normals.Vertices[AccessIdx].y = -CurrentNormal.z;
            MeshObjectInstance->Normals.Vertices[AccessIdx].z = CurrentNormal.x;
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];

            MeshObjectInstance->Tangents.Vertices[AccessIdx].x = CurrentTangent.x;
            MeshObjectInstance->Tangents.Vertices[AccessIdx].y = -CurrentTangent.z;
            MeshObjectInstance->Tangents.Vertices[AccessIdx].z = CurrentTangent.x;
            MeshObjectInstance->Bitangents.Vertices[AccessIdx].x = CurrentBiTangent.x;
            MeshObjectInstance->Bitangents.Vertices[AccessIdx].y = -CurrentBiTangent.z;
            MeshObjectInstance->Bitangents.Vertices[AccessIdx].z = CurrentBiTangent.x;
        }
    }
}

template<typename T>
void AssetManager::CalculateTB(const aiMesh* const Mesh, size_t IndexStartIdx, T* MeshObjectInstance)
{
    if (Mesh->HasTangentsAndBitangents())
    {
        // Use Tangent And Bitangent of File
    }
    else
    {
        for (size_t IndexIdx = IndexStartIdx; IndexIdx < MeshObjectInstance->Indices.size(); IndexIdx += 3)
        {
            auto& p0 = MeshObjectInstance->Positions.Vertices[MeshObjectInstance->Indices[IndexIdx]];
            auto& p1 = MeshObjectInstance->Positions.Vertices[MeshObjectInstance->Indices[IndexIdx + 1]];
            auto& p2 = MeshObjectInstance->Positions.Vertices[MeshObjectInstance->Indices[IndexIdx + 2]];

            auto& uv0 = MeshObjectInstance->UVTextures.Vertices[MeshObjectInstance->Indices[IndexIdx]];
            auto& uv1 = MeshObjectInstance->UVTextures.Vertices[MeshObjectInstance->Indices[IndexIdx + 1]];
            auto& uv2 = MeshObjectInstance->UVTextures.Vertices[MeshObjectInstance->Indices[IndexIdx + 2]];

            auto& n0 = MeshObjectInstance->Normals.Vertices[MeshObjectInstance->Indices[IndexIdx]];
            auto& n1 = MeshObjectInstance->Normals.Vertices[MeshObjectInstance->Indices[IndexIdx + 1]];
            auto& n2 = MeshObjectInstance->Normals.Vertices[MeshObjectInstance->Indices[IndexIdx + 2]];

            auto& t0 = MeshObjectInstance->Tangents.Vertices[MeshObjectInstance->Indices[IndexIdx]];
            auto& t1 = MeshObjectInstance->Tangents.Vertices[MeshObjectInstance->Indices[IndexIdx + 1]];
            auto& t2 = MeshObjectInstance->Tangents.Vertices[MeshObjectInstance->Indices[IndexIdx + 2]];

            auto& bt0 = MeshObjectInstance->Bitangents.Vertices[MeshObjectInstance->Indices[IndexIdx]];
            auto& bt1 = MeshObjectInstance->Bitangents.Vertices[MeshObjectInstance->Indices[IndexIdx + 1]];
            auto& bt2 = MeshObjectInstance->Bitangents.Vertices[MeshObjectInstance->Indices[IndexIdx + 2]];

            MathematicalHelper::GetTangentBitangent(
                p0,
                p1,
                p2,
                uv0,
                uv1,
                uv2,
                n0,
                n1,
                n2,
                t0,
                t1,
                t2,
                bt0,
                bt1,
                bt2
            );
        }

    }
}