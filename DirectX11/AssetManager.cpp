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

#include "DirectXTexEXR.h"

#include <filesystem>
#include <regex>
#include <set>

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
            LoadAssetFileHelper(InputAssetFile, ManagingStaticMeshes, AssetName, true);
            break;
        case EAssetType::SkeletalMesh:
            LoadAssetFileHelper(InputAssetFile, ManagingSkeletalMeshes, AssetName, true);
            break;
        case EAssetType::Bone:
            LoadAssetFileHelper(InputAssetFile, ManagingBones, AssetName, true);
            break;
        case EAssetType::Map:
            LoadAssetFileHelper(InputAssetFile, ManagingMaps, AssetName, this, true);
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

    size_t LODCount = GetTotalLODCountFromScene(Scene);

    if (HasBone(Scene))
    {
        shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetLoaded = make_shared<SkeletalMeshAsset>(AssetName, false);
        shared_ptr<BoneAsset> BoneAssetLoaded = make_shared<BoneAsset>(AssetName, false);
        SkeletalMeshAssetLoaded->SetLODCount(LODCount);

        SkeletalMeshAssetLoaded->SetLinkedBoneAsset(BoneAssetLoaded);
        BoneAsset* BoneAPtr = BoneAssetLoaded.get();
        LoadBone(Scene, BoneAPtr);

        SkeletalMeshAsset* SkeletalMeshAPtr = SkeletalMeshAssetLoaded.get();
        ProcessNodeForMesh(IsGltf, Scene, RootNode, SkeletalMeshAPtr, BoneAPtr, RootTransform);

        // Serialize Bone Asset
        AddToManagingContainer(ManagingBones, BoneAssetLoaded);

        SkeletalMeshAssetLoaded->Initialize();
        AddToManagingContainer(ManagingSkeletalMeshes, SkeletalMeshAssetLoaded);
    }
    else
    {
        shared_ptr<StaticMeshAsset> StaticMeshLoaded = make_shared<StaticMeshAsset>(AssetName, false);
        StaticMeshLoaded->SetLODCount(LODCount);

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

std::shared_ptr<NormalTextureAsset> AssetManager::GetManagingNormalTexture(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingNormalTextures, MapAssetName);
}

std::shared_ptr<EXRTextureAsset> AssetManager::GetManagingEXRTexture(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingEXRTextures, MapAssetName);
}

std::shared_ptr<DDSTextureAsset> AssetManager::GetManagingDDSTexture(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingDDSTextures, MapAssetName);
}

BaseMeshAsset* AssetManager::GetManagingBaseMesh(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingBaseMeshes, MapAssetName);
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
        const size_t LODLevel = GetLODLevelFromMeshName(CurrentMesh->mName);
        LoadMeshElement(IsGltf, CurrentMesh, StaticMesh, LODLevel, CurrentMatrix);
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
        const size_t LODLevel = GetLODLevelFromMeshName(CurrentMesh->mName);
        LoadMeshElement(IsGltf, CurrentMesh, SkeletalMesh, LODLevel, BoneA, CurrentMatrix);
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

void AssetManager::RestructBaseVertices(const unsigned int& NumVertices, ANBTMeshAsset* MeshAssetIn, const size_t LodLevel)
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

    MeshAssetIn->PositionsPerLOD[LodLevel].Vertices.insert(MeshAssetIn->PositionsPerLOD[LodLevel].Vertices.end(), TempPositions.begin(), TempPositions.end());
    MeshAssetIn->UVTexturesPerLOD[LodLevel].Vertices.insert(MeshAssetIn->UVTexturesPerLOD[LodLevel].Vertices.end(), TempUVTextures.begin(), TempUVTextures.end());
    MeshAssetIn->NormalsPerLOD[LodLevel].Vertices.insert(MeshAssetIn->NormalsPerLOD[LodLevel].Vertices.end(), TempNormals.begin(), TempNormals.end());
    MeshAssetIn->TangentsPerLOD[LodLevel].Vertices.insert(MeshAssetIn->TangentsPerLOD[LodLevel].Vertices.end(), TempTangents.begin(), TempTangents.end());
    MeshAssetIn->BitangentsPerLOD[LodLevel].Vertices.insert(MeshAssetIn->BitangentsPerLOD[LodLevel].Vertices.end(), TempBitangents.begin(), TempBitangents.end());
}

size_t AssetManager::GetTotalLODCountFromScene(const aiScene* const Scene)
{
    size_t Lodlevel = 0;

    for (UINT meshIdx = 0; meshIdx < Scene->mNumMeshes; ++meshIdx)
    {
        aiMesh* CurrentMesh = Scene->mMeshes[meshIdx];
        Lodlevel = max(Lodlevel, GetLODLevelFromMeshName(CurrentMesh->mName));
    }
    return Lodlevel + 1;
}

size_t AssetManager::GetLODLevelFromMeshName(const aiString& MeshName)
{
    size_t Lodlevel = 0;
    regex LODMeshNamePatter("_LOD(\\d+)");
    smatch Match;

    const string MeshNameStr = string(MeshName.C_Str());
    if (regex_search(MeshNameStr, Match, LODMeshNamePatter))
    {
        std::string lod_number = Match[1];
        Lodlevel = max(Lodlevel, static_cast<size_t>(stoul(lod_number)));
    }

    return Lodlevel;
}

void AssetManager::LoadMeshElement(
    bool IsGltf,
    const aiMesh* const Mesh,
    StaticMeshAsset* StaticMesh,
    const size_t LodLevel,
    const DirectX::XMMATRIX& ParentMatrix
)
{
    size_t VertexStartIdx = StaticMesh->PositionsPerLOD[LodLevel].Vertices.size();
    size_t IndicesStartIdx = StaticMesh->IndicesPerLOD[LodLevel].Indices.size();

    RestructBaseVertices(Mesh->mNumVertices, StaticMesh, LodLevel);

    // Load Position
    LoadPosition(Mesh, VertexStartIdx, StaticMesh->PositionsPerLOD[LodLevel].Vertices, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, StaticMesh->UVTexturesPerLOD[LodLevel].Vertices);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, StaticMesh, LodLevel) : LoadTBN(Mesh, VertexStartIdx, StaticMesh, LodLevel);
 
    // Load Index;
    LoadIndices(Mesh, VertexStartIdx, StaticMesh->IndicesPerLOD[LodLevel].Indices);

    CalculateTB(Mesh, IndicesStartIdx, StaticMesh, LodLevel);
}

void AssetManager::LoadMeshElement(
    bool IsGltf,
    const aiMesh* const Mesh,
    SkeletalMeshAsset* SkeletalMesh,
    const size_t LodLevel,
    BoneAsset* BoneA,
    const DirectX::XMMATRIX& ParentMatrix
)
{
    size_t VertexStartIdx = SkeletalMesh->PositionsPerLOD[LodLevel].Vertices.size();
    size_t IndicesStartIdx = SkeletalMesh->IndicesPerLOD[LodLevel].Indices.size();

    RestructBaseVertices(Mesh->mNumVertices, SkeletalMesh, LodLevel);

    vector<XMFLOAT4> TempBlendWeight;
    vector<XMINT4> TempBlendIndex;

    TempBlendWeight.resize(Mesh->mNumVertices);
    TempBlendIndex.resize(Mesh->mNumVertices, XMINT4(-1, -1, -1, -1));

    SkeletalMesh->BlendWeightPerLOD[LodLevel].Vertices.insert(SkeletalMesh->BlendWeightPerLOD[LodLevel].Vertices.end(), TempBlendWeight.begin(), TempBlendWeight.end());
    SkeletalMesh->BlendIndexPerLOD[LodLevel].Vertices.insert(SkeletalMesh->BlendIndexPerLOD[LodLevel].Vertices.end(), TempBlendIndex.begin(), TempBlendIndex.end());


    // Load Position
    LoadPosition(Mesh, VertexStartIdx, SkeletalMesh->PositionsPerLOD[LodLevel].Vertices, ParentMatrix);

    // Load Texture Coordinate
    LoadTextureCoord(Mesh, VertexStartIdx, SkeletalMesh->UVTexturesPerLOD[LodLevel].Vertices);

    // Load TBN
    IsGltf ? LoadTBNAsGltf(Mesh, VertexStartIdx, SkeletalMesh, LodLevel) : LoadTBN(Mesh, VertexStartIdx, SkeletalMesh, LodLevel);
    
    // Load Index;
    LoadIndices(Mesh, VertexStartIdx, SkeletalMesh->IndicesPerLOD[LodLevel].Indices);

    // Load BlendWeight
    LoadBlendWeightAndIndex(Mesh, SkeletalMesh->BlendWeightPerLOD[LodLevel].Vertices, SkeletalMesh->BlendIndexPerLOD[LodLevel].Vertices);

    CalculateTB(Mesh, IndicesStartIdx, SkeletalMesh, LodLevel);
}

void AssetManager::LoadPosition(
    const aiMesh* const Mesh,
    size_t VertexStartIdx,
    std::vector<XMFLOAT3>& Postions,
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
            memcpy(&/*MeshObjectInstance->Positions.Vertices*/Postions[AccessIdx], &TransformedVertex, sizeof(aiVector3D));
        }
    }
}

void AssetManager::LoadBone(
    const aiScene* const Scene,
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
            }
            // Load Bone Hierachy
            ProcessNodeForBone(Scene, Scene->mRootNode, BoneAsset);
        }
    }
}

void AssetManager::LoadTextureCoord(
    const aiMesh* const Mesh, 
    size_t VertexStartIdx, 
    vector<XMFLOAT2>& UVTextures
)
{
    if (Mesh->HasTextureCoords(0))
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTextureCoord = Mesh->mTextureCoords[0][VertexIdx];
            memcpy(&UVTextures[AccessIdx], &CurrentTextureCoord, sizeof(aiVector2D));
        }
    }
}

void AssetManager::LoadBlendWeightAndIndex(
    const aiMesh* const Mesh, 
    vector<XMFLOAT4>& BlendWeight, 
    vector<XMINT4>& BlendIndex
)
{
    // Load Bone Table And Weight
    for (UINT BoneIdx = 0; BoneIdx < Mesh->mNumBones; ++BoneIdx)
    {
        aiBone* CurrentBone = Mesh->mBones[BoneIdx];
        for (UINT WeightIdx = 0; WeightIdx < CurrentBone->mNumWeights; ++WeightIdx)
        {
            aiVertexWeight& VertexWeight = CurrentBone->mWeights[WeightIdx];

            XMFLOAT4& CurrentBlendWeight = BlendWeight[VertexWeight.mVertexId];
            XMINT4& CurrentBlendIndex = BlendIndex[VertexWeight.mVertexId];

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
}

void AssetManager::LoadIndices(
    const aiMesh* const Mesh,
    size_t VertexStartIdx,
    std::vector<uint32_t>& IndicesIn
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

    // TODO : 위상 정렬등을 활용하여 Asset의 로딩 순서 결정하기 
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
    T* MeshObjectInstance,
    const size_t& LodLevel
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            memcpy(&MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx], &CurrentNormal, sizeof(aiVector3D));
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];
            memcpy(&MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx], &CurrentTangent, sizeof(aiVector3D));
            memcpy(&MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[AccessIdx], &CurrentBiTangent, sizeof(aiVector3D));
        }
    }
}

template<typename T>
void AssetManager::LoadTBNAsGltf(
    const aiMesh* const Mesh,
    size_t VertexStartIdx, 
    T* MeshObjectInstance,
    const size_t& LodLevel
)
{
    if (Mesh->HasNormals())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentNormal = Mesh->mNormals[VertexIdx];
            MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx].x = CurrentNormal.x;
            MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx].y = -CurrentNormal.z;
            MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx].z = CurrentNormal.x;
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];
            aiVector3D& CurrentBiTangent = Mesh->mBitangents[VertexIdx];

            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].x = CurrentTangent.x;
            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].y = -CurrentTangent.z;
            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].z = CurrentTangent.x;
            MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[AccessIdx].x = CurrentBiTangent.x;
            MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[AccessIdx].y = -CurrentBiTangent.z;
            MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[AccessIdx].z = CurrentBiTangent.x;
        }
    }
}

template<typename T>
void AssetManager::CalculateTB(const aiMesh* const Mesh, size_t IndexStartIdx, T* MeshObjectInstance, const size_t& LodLevel)
{
    if (Mesh->HasTangentsAndBitangents())
    {
        // Use Tangent And Bitangent of File
    }
    else
    {
        for (size_t IndexIdx = IndexStartIdx; IndexIdx < MeshObjectInstance->IndicesPerLOD[LodLevel].Indices.size(); IndexIdx += 3)
        {
            auto& p0 = MeshObjectInstance->PositionsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx]];
            auto& p1 = MeshObjectInstance->PositionsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 1]];
            auto& p2 = MeshObjectInstance->PositionsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 2]];

            auto& uv0 = MeshObjectInstance->UVTexturesPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx]];
            auto& uv1 = MeshObjectInstance->UVTexturesPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 1]];
            auto& uv2 = MeshObjectInstance->UVTexturesPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 2]];

            auto& n0 = MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx]];
            auto& n1 = MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 1]];
            auto& n2 = MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 2]];

            auto& t0 = MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx]];
            auto& t1 = MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 1]];
            auto& t2 = MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 2]];

            auto& bt0 = MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx]];
            auto& bt1 = MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 1]];
            auto& bt2 = MeshObjectInstance->BitangentsPerLOD[LodLevel].Vertices[MeshObjectInstance->IndicesPerLOD[LodLevel].Indices[IndexIdx + 2]];

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