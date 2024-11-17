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

#include "BaseTextureAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"
#include "MaterialAsset.h"
#include "AnimationAsset.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "MathematicalHelper.h"
#include "StringHelper.h"
#include "StaticAssertHelper.h"

#include "DirectXTexEXR.h"

#include <regex>
#include <map>
#include <set>

using namespace std;
using namespace DirectX;
using namespace filesystem;

unordered_map<string, EFileType> AssetManager::FileExtensionToType
{
    { ".gltf", EFileType::ModelFile },
    { ".fbx", EFileType::ModelFile },
    { ".obj", EFileType::ModelFile },
    { ".jpg", EFileType::BasicTextureFile },
    { ".jpeg", EFileType::BasicTextureFile},
    { ".png", EFileType::BasicTextureFile },
    { ".exr", EFileType::EXRTextureFile },
    { ".dds", EFileType::DDSTextureFile },
};

AssetManager::AssetManager()
{
    App::GAssetManager = this;
    PreloadAssets();
}

AssetManager::~AssetManager()
{
    SerializeModifiedAsset(ManagingBones);
    SerializeModifiedAsset(ManagingStaticMeshes);
    SerializeModifiedAsset(ManagingSkeletalMeshes);
    SerializeModifiedAsset(ManagingBasicTextures);
    SerializeModifiedAsset(ManagingEXRTextures);
    SerializeModifiedAsset(ManagingDDSTextures);
    SerializeModifiedAsset(ManagingMaterials);
}

void AssetManager::LoadAssetFromFile(const std::string& FilePathIn)
{
    path FilePath = FilePathIn;
    const string FileName = FilePath.stem().string();
    const string FileExtension = FilePath.extension().string();

    if (FileExtensionToType.find(FileExtension) != FileExtensionToType.end())
    {
        switch (FileExtensionToType[FileExtension])
        {
        case EFileType::ModelFile:
            LoadModelAssetFromFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::BasicTextureFile:
            LoadBasicTextureAssetFromFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::EXRTextureFile:
            LoadEXRTextureAssetFromFile(FilePathIn, FileName, FileExtension);
            break;
        case EFileType::DDSTextureFile:
            LoadDDSTextureAssetFromFile(FilePathIn, FileName, FileExtension);
            break;
        default:
            break;
        }
    }
}


void AssetManager::LoadModelAssetFromFile(const string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn)
{
    if (FileNameToAssetNames.find(FileNameIn) == FileNameToAssetNames.end())
    {
        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(
            StringHelper::ConvertACPToUTF8(FilePathIn),
            aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_ConvertToLeftHanded
        );

        if (pScene)
        {
            CurrentModelsMaterials.clear();

            if (pScene->HasMaterials())
            {
                LoadMaterialAssetFromFile(FilePathIn, FileNameIn, pScene);
            }

            // Load Mesh
            if (pScene->HasMeshes())
            {
                LoadMeshAssetFromFile(FileExtensionIn == ".gltf", FileNameIn, pScene);
            }

            if (pScene->HasAnimations())
            {
                LoadAnimationAssetFromFile(FileNameIn, pScene);
            }
        }
    }
}

void AssetManager::LoadBasicTextureAssetFromFile(
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
            shared_ptr<BaseTextureAsset> TextureAssetLoaded = make_shared<BaseTextureAsset>(FileNameIn, ImageBuffer, WidthOut, HeightOut);
            SerailizeAndAddToContainer(TextureAssetLoaded);
        }
        fclose(FileHandle);

    }
    else
    {
    }
}

void AssetManager::LoadEXRTextureAssetFromFile(
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
        SerailizeAndAddToContainer(TextureAssetLoaded);
    }
}

void AssetManager::LoadDDSTextureAssetFromFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn)
{
    ScratchImage scratch;
    TexMetadata metaData;
    HRESULT hResult = LoadFromDDSFile(StringHelper::ConvertACPToWString(FilePathIn).c_str(), DDS_FLAGS_NONE, &metaData, scratch);
    if (!FAILED(hResult))
    {
        shared_ptr<DDSTextureAsset> TextureAssetLoaded = make_shared<DDSTextureAsset>(FileNameIn, scratch, metaData);
        SerailizeAndAddToContainer(TextureAssetLoaded);
    }
}

void AssetManager::LoadMeshAssetFromFile(bool IsGltf, const string& AssetName, const aiScene* const Scene)
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
        SerailizeAndAddToContainer(BoneAssetLoaded);

        SkeletalMeshAssetLoaded->Initialize();
        SkeletalMeshAssetLoaded->SetDefaultMaterialAssets(CurrentModelsMaterials);
        SerailizeAndAddToContainer(SkeletalMeshAssetLoaded);
    }
    else
    {
        shared_ptr<StaticMeshAsset> StaticMeshLoaded = make_shared<StaticMeshAsset>(AssetName, false);
        StaticMeshLoaded->SetLODCount(LODCount);

        ProcessNodeForMesh(IsGltf, Scene, RootNode, (StaticMeshAsset*)StaticMeshLoaded.get(), RootTransform);

        StaticMeshLoaded->Initialize();
        StaticMeshLoaded->SetDefaultMaterialAssets(CurrentModelsMaterials);
        SerailizeAndAddToContainer(StaticMeshLoaded);
    }
}

void AssetManager::LoadMaterialAssetFromFile(const string& FilePath, const string& AssetName, const aiScene* const Scene)
{
    for (size_t material_idx = 0; material_idx < Scene->mNumMaterials; ++material_idx)
    {
        aiMaterial* pMaterial = Scene->mMaterials[material_idx];
        shared_ptr<MaterialAsset> MaterialAssetLoaded = make_shared<MaterialAsset>(pMaterial->GetName().C_Str(), false);

        aiString MaterialName = pMaterial->GetName();
        if (pMaterial->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
        {
            MaterialAssetLoaded->SetAmbientOcculusionTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_AMBIENT_OCCLUSION));
        }
        if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            MaterialAssetLoaded->SetSpecularTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_SPECULAR));
        }
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            MaterialAssetLoaded->SetDiffuseTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_DIFFUSE));
        }
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
        {
            MaterialAssetLoaded->SetRoughnessTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_DIFFUSE_ROUGHNESS));
        }
        if (pMaterial->GetTextureCount(aiTextureType_METALNESS) > 0)
        {
            MaterialAssetLoaded->SetMetalicTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_METALNESS));
        }
        if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            MaterialAssetLoaded->SetNormalTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_NORMALS));
        }
        if (pMaterial->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            MaterialAssetLoaded->SetHeightTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_HEIGHT));
        }
        if (pMaterial->GetTextureCount(aiTextureType_EMISSIVE) > 0)
        {
            MaterialAssetLoaded->SetEmissiveTextureAsset(LoadBasicTextureFromMaterial(Scene, pMaterial, aiTextureType_EMISSIVE));
        }
        SerailizeAndAddToContainer(MaterialAssetLoaded);
        CurrentModelsMaterials.push_back(MaterialAssetLoaded);
    }
}

shared_ptr<BaseTextureAsset> AssetManager::LoadBasicTextureFromMaterial(const aiScene* const Scene, aiMaterial* MaterialIn, aiTextureType TextureTypeIn)
{
    aiString aiTexturePath;
    shared_ptr<BaseTextureAsset> TextureAssetLoaded;
    if (MaterialIn->GetTexture(TextureTypeIn, 0, &aiTexturePath) == aiReturn_SUCCESS)
    {
        const string TexturePathString = aiTexturePath.C_Str();
        path TexturePath = TexturePathString;
        const string TextureName = TexturePath.stem().string();

        const aiTexture* Texture = Scene->GetEmbeddedTexture(TexturePathString.c_str());
        if (Texture != nullptr)
        {
            int WidthOut, HeightOut, ChannelOut;
            stbi_uc* ImageBuffer = stbi_load_from_memory((const stbi_uc*)Texture->pcData, Texture->mWidth, &WidthOut, &HeightOut, &ChannelOut, 4);
            if (ImageBuffer != nullptr)
            {
                TextureAssetLoaded = make_shared<BaseTextureAsset>(TextureName, ImageBuffer, WidthOut, HeightOut);
                SerailizeAndAddToContainer(TextureAssetLoaded);
                stbi_image_free(ImageBuffer);
            }
        }
    }
    return TextureAssetLoaded;
}

void AssetManager::LoadAnimationAssetFromFile(const string& AssetName, const aiScene* const Scene)
{
    for (UINT AnimIdx = 0; AnimIdx < Scene->mNumAnimations; AnimIdx++)
    {
        aiAnimation* Animation = Scene->mAnimations[AnimIdx];
        const string AnimName = Scene->mNumAnimations > 1 ? format("{}_{}", AssetName, AnimIdx) : AssetName;
        shared_ptr<AnimationAsset> AnimAsset = make_shared<AnimationAsset>(AnimName, false, Animation->mDuration, Animation->mTicksPerSecond);

        LoadAnimationChannels(Animation, AnimAsset.get());
        SerailizeAndAddToContainer(AnimAsset);
    }
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
    const string AssetType = AssetNameIn.substr(AssetNameIn.find_last_of("_") + 1);

    if (AssetType == BaseMeshAsset::BaseMeshAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingBaseMeshes, AssetNameIn);
    }
    else if (AssetType == StaticMeshAsset::StaticMeshAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingStaticMeshes, AssetNameIn).get();
    }
    else if (AssetType == SkeletalMeshAsset::SkeletalMeshAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingSkeletalMeshes, AssetNameIn).get();
    }
    else if (AssetType == BoneAsset::BoneAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingBones, AssetNameIn).get();
    }
    else if (AssetType == MapAsset::MapAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingMaps, AssetNameIn).get();
    }
    else if (AssetType == MaterialAsset::MaterialAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingMaterials, AssetNameIn).get();
    }
    else if (AssetType == BaseTextureAsset::BaseTextureAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingBasicTextures, AssetNameIn).get();
    }
    else if (AssetType == EXRTextureAsset::EXRTextureAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingEXRTextures, AssetNameIn).get();
    }
    else if (AssetType == DDSTextureAsset::DDSTextureAssetKind)
    {
        Result = GetManagingAssetHelper(ManagingDDSTextures, AssetNameIn).get();
    }
    else if (AssetType == AnimationAsset::AnimationKind)
    {
        Result = GetManagingAssetHelper(ManagingAnimations, AssetNameIn).get();
    }
    else
    {

    }
    return Result;
}

std::shared_ptr<MapAsset> AssetManager::GetManagingMap(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingMaps, AssetName);
}

std::shared_ptr<BoneAsset> AssetManager::GetManagingBone(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingBones, AssetName);
}

std::shared_ptr<StaticMeshAsset> AssetManager::GetManagingStaticMesh(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingStaticMeshes, AssetName);
}

std::shared_ptr<SkeletalMeshAsset> AssetManager::GetManagingSkeletalMesh(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingSkeletalMeshes, AssetName);
}

std::shared_ptr<BaseTextureAsset> AssetManager::GetManagingBasicTexture(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingBasicTextures, AssetName);
}

std::shared_ptr<EXRTextureAsset> AssetManager::GetManagingEXRTexture(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingEXRTextures, AssetName);
}

std::shared_ptr<DDSTextureAsset> AssetManager::GetManagingDDSTexture(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingDDSTextures, AssetName);
}

std::shared_ptr<MaterialAsset> AssetManager::GetManagingMaterial(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingMaterials, AssetName);
}

std::shared_ptr<AnimationAsset> AssetManager::GetManagingAnimation(const std::string AssetName)
{
    return GetManagingAssetHelper(ManagingAnimations, AssetName);
}

BaseMeshAsset* AssetManager::GetManagingBaseMesh(const std::string MapAssetName)
{
    return GetManagingAssetHelper(ManagingBaseMeshes, MapAssetName);
}

template<typename T>
void AssetManager::SerailizeAndAddToContainer(std::unordered_map<std::string, T>& ManagingContainer, T& AddedAsset)
{
    AddedAsset->Serialize();
    ManagingContainer.emplace(AddedAsset->GetAssetName(), AddedAsset);

    AssetChangedEvent.Invoke();
}

template<typename T>
void AssetManager::SerializeModifiedAsset(unordered_map<string, T>& ManagingContainer)
{
    for (auto& ManagingAsset : ManagingContainer)
    {
        const T& ManagingAssetCached = ManagingAsset.second;
        if (ManagingAssetCached->GetIsModified())
        {
            ManagingAssetCached->Serialize();
        }
    }
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
    regex LODMeshNamePatter("\\.(\\d+)");
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

    StaticMesh->IndexCountsForPartPerLOD[LodLevel].push_back(StaticMesh->IndicesPerLOD[LodLevel].Indices.size() - IndicesStartIdx);
    StaticMesh->IndexOffsetsForPartPerLOD[LodLevel].push_back(IndicesStartIdx);
    StaticMesh->MaterialIndexPerLOD[LodLevel].push_back(Mesh->mMaterialIndex);
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

    TempBlendWeight.resize(Mesh->mNumVertices/*, XMFLOAT4(0.f, 0.f, 0.f, 0.f)*/);
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
    LoadBlendWeightAndIndex(Mesh, VertexStartIdx, SkeletalMesh->BlendWeightPerLOD[LodLevel].Vertices, SkeletalMesh->BlendIndexPerLOD[LodLevel].Vertices);

    CalculateTB(Mesh, IndicesStartIdx, SkeletalMesh, LodLevel);

    SkeletalMesh->IndexCountsForPartPerLOD[LodLevel].push_back(SkeletalMesh->IndicesPerLOD[LodLevel].Indices.size() - IndicesStartIdx);
    SkeletalMesh->IndexOffsetsForPartPerLOD[LodLevel].push_back(IndicesStartIdx);
    SkeletalMesh->MaterialIndexPerLOD[LodLevel].push_back(Mesh->mMaterialIndex);
}

void AssetManager::LoadPosition(
    const aiMesh* const Mesh,
    const size_t& VertexStartIdx,
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
            memcpy(&Postions[AccessIdx], &TransformedVertex, sizeof(aiVector3D));
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
                BoneAsset->AddBone(CurrentBone->mName.C_Str(), BoneIdx, XMMatrixTranspose(DirectX::XMLoadFloat4x4(&TempMatrix)));
            }
            // Load Bone Hierachy
            ProcessNodeForBone(Scene, Scene->mRootNode, BoneAsset);
        }
    }
}

void AssetManager::LoadAnimationChannels(const aiAnimation* const Animation, AnimationAsset* AnimAsset)
{
    for (UINT ChannelIdx = 0; ChannelIdx < Animation->mNumChannels; ++ChannelIdx)
    {
        aiNodeAnim* NodeChannel = Animation->mChannels[ChannelIdx];

        const string& ChannelName = NodeChannel->mNodeName.C_Str();

        AnimationChannel AnimChannel;
        LoadPositionKeys(NodeChannel, AnimChannel);
        LoadQuaternionKeys(NodeChannel, AnimChannel);
        LoadScaleKeys(NodeChannel, AnimChannel);

        AnimAsset->AddAnimationChannel(ChannelName, move(AnimChannel));
    }
}

void AssetManager::LoadPositionKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel)
{
    for (UINT PositionIdx = 0; PositionIdx < NodeChannel->mNumPositionKeys; ++PositionIdx)
    {
        const aiVectorKey& PositionKey = NodeChannel->mPositionKeys[PositionIdx];
        AnimChannel.AddPositionKey(
            static_cast<float>(PositionKey.mTime), 
            XMVectorSet(PositionKey.mValue.x, PositionKey.mValue.y, PositionKey.mValue.z, 1.f)
        );
    }
}

void AssetManager::LoadQuaternionKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel)
{
    for (UINT QuaternionIdx = 0; QuaternionIdx < NodeChannel->mNumRotationKeys; ++QuaternionIdx)
    {
        const aiQuatKey& QuaternionKey = NodeChannel->mRotationKeys[QuaternionIdx];
        AnimChannel.AddQuaternionKey(
            static_cast<float>(QuaternionKey.mTime),
            XMVectorSet(QuaternionKey.mValue.x, QuaternionKey.mValue.y, QuaternionKey.mValue.z, QuaternionKey.mValue.w)
        );
    }
}

void AssetManager::LoadScaleKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel)
{
    for (UINT ScaleIdx = 0; ScaleIdx < NodeChannel->mNumScalingKeys; ++ScaleIdx)
    {
        const aiVectorKey& ScaleKey = NodeChannel->mScalingKeys[ScaleIdx];
        AnimChannel.AddScaleKey(
            static_cast<float>(ScaleKey.mTime),
            XMVectorSet(ScaleKey.mValue.x, ScaleKey.mValue.y, ScaleKey.mValue.z, 0.f)
        );
    }
}

void AssetManager::LoadTextureCoord(
    const aiMesh* const Mesh, 
    const size_t& VertexStartIdx,
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
    const size_t& VertexStartIdx,
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

            const size_t AccessIdx = VertexStartIdx + VertexWeight.mVertexId;

            XMFLOAT4& CurrentBlendWeight = BlendWeight[AccessIdx];
            XMINT4& CurrentBlendIndex = BlendIndex[AccessIdx];

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
    const size_t& VertexStartIdx,
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
        SerailizeAndAddToContainer(ManagingBaseMeshes, BaseMeshAssetInstance);
    }

    path AssetPath = path(AssetOutPath);
    if (!exists(AssetPath) && create_directories(AssetPath)) {/* Do Nothing But Make Directory */ };

    // Load Asset File
    vector<string> AssetFilePaths;
    TraverseDirectory(AssetPath, AssetFilePaths);

    path BaseAssetFilePath = path(BaseAssetOutPath);
    if (!exists(BaseAssetFilePath) && create_directories(BaseAssetFilePath)) {/* Do Nothing But Make Directory */ };

    // Load Base Asset File
    for (const auto& entry : directory_iterator(BaseAssetFilePath))
    {
        const path BaseFilePath = entry.path();
        if (entry.is_regular_file())
        {
            LoadAssetFromFile(entry.path().string());
        }
        else;
    }

    LoadAssetWithTopologySorting(AssetFilePaths);
}

void AssetManager::TraverseDirectory(const path& PathIn, vector<string>& AssetFilePathsIn)
{
    for (const auto& entry : directory_iterator(PathIn))
    {
        const path filePath = entry.path();
        if (entry.is_regular_file())
        {
            if (filePath.extension() == AssetExtension)
            {
                AssetFilePathsIn.push_back(filePath.string());
            }
        }
        else if (entry.is_directory())
        {
            TraverseDirectory(filePath, AssetFilePathsIn);
        }
        else;
    }
}

void AssetManager::LoadAssetWithTopologySorting(const vector<string>& AssetPathsIn)
{
    // 위상 정렬을 통한 Asset Loading Priority 결정
    unordered_map<string, size_t> AssetLoadPriorities = AssetPriorityManagerInstance.GetAssetLoadPriorities();
    map<size_t, vector<SAssetPreloadArgs>> AssetLoadArgsWithPriorities;

    for (const string& AssetPathIn : AssetPathsIn)
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
            string AssetType;
            AAssetFile::DeserializeString(AssetType, InputAssetFile);

            long LastReadPoint = ftell(InputAssetFile);

            fclose(InputAssetFile);

            SAssetPreloadArgs AssetPreLoadArgs;
            AssetPreLoadArgs.AssetName = AssetName;
            AssetPreLoadArgs.AssetPath = AssetPathIn;
            AssetPreLoadArgs.LastReadPoint = LastReadPoint;
            AssetPreLoadArgs.AssetType = AssetType;

            AssetLoadArgsWithPriorities[AssetLoadPriorities[AssetType]].push_back(AssetPreLoadArgs);
        }
    }

    for (auto& AssetLoadArgsVector : AssetLoadArgsWithPriorities)
    {
        for (const SAssetPreloadArgs& AssetPreloadArgs : AssetLoadArgsVector.second)
        {
            FILE* InputAssetFile = nullptr;
            fopen_s(&InputAssetFile, AssetPreloadArgs.AssetPath.c_str(), "rb");

            if (InputAssetFile != nullptr)
            {
                fseek(InputAssetFile, AssetPreloadArgs.LastReadPoint, SEEK_SET);

                const string& AssetType = AssetPreloadArgs.AssetType;

                if (AssetType == BaseMeshAsset::BaseMeshAssetKind)
                {

                }
                else if (AssetType == StaticMeshAsset::StaticMeshAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingStaticMeshes, AssetPreloadArgs.AssetName, true);
                }
                else if (AssetType == SkeletalMeshAsset::SkeletalMeshAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingSkeletalMeshes, AssetPreloadArgs.AssetName, true);
                }
                else if (AssetType == BoneAsset::BoneAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingBones, AssetPreloadArgs.AssetName, true);
                }
                else if (AssetType == MapAsset::MapAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingMaps, AssetPreloadArgs.AssetName, true);
                }
                else if (AssetType == BaseTextureAsset::BaseTextureAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingBasicTextures, AssetPreloadArgs.AssetName);
                }
                else if (AssetType == EXRTextureAsset::EXRTextureAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingEXRTextures, AssetPreloadArgs.AssetName);
                }
                else if (AssetType == DDSTextureAsset::DDSTextureAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingDDSTextures, AssetPreloadArgs.AssetName);
                }
                else if (AssetType == MaterialAsset::MaterialAssetKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingMaterials, AssetPreloadArgs.AssetName, true);
                }
                else if (AssetType == AnimationAsset::AnimationKind)
                {
                    LoadAssetHelper(InputAssetFile, ManagingAnimations, AssetPreloadArgs.AssetName, true);
                }
                else
                {

                }

                fclose(InputAssetFile);
            }
        }
    }
}




template<typename T, typename ...Args>
void AssetManager::LoadAssetHelper(
    FILE* FileIn,
    std::unordered_map<std::string, std::shared_ptr<T>>& ManagingContainer,
    const std::string& AssetName,
    Args... AssetConstructArgs
)
{
    shared_ptr<T> AssetFile = make_shared<T>(AssetName, AssetConstructArgs...);
    AssetFile->Deserialize(FileIn);
    ManagingContainer.emplace(AssetName, AssetFile);
}


template<typename T>
void AssetManager::LoadTBN(
    const aiMesh* const Mesh, 
    const size_t& VertexStartIdx,
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
            memcpy(&MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx], &CurrentTangent, sizeof(aiVector3D));
        }
    }
}

template<typename T>
void AssetManager::LoadTBNAsGltf(
    const aiMesh* const Mesh,
    const size_t& VertexStartIdx,
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
            MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx].y = CurrentNormal.y;
            MeshObjectInstance->NormalsPerLOD[LodLevel].Vertices[AccessIdx].z = CurrentNormal.z;
        }
    }

    if (Mesh->HasTangentsAndBitangents())
    {
        for (size_t VertexIdx = 0; VertexIdx < Mesh->mNumVertices; ++VertexIdx)
        {
            const size_t AccessIdx = VertexStartIdx + VertexIdx;
            aiVector3D& CurrentTangent = Mesh->mTangents[VertexIdx];

            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].x = CurrentTangent.x;
            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].y = CurrentTangent.y;
            MeshObjectInstance->TangentsPerLOD[LodLevel].Vertices[AccessIdx].z = CurrentTangent.z;
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
        XMFLOAT3 bt0;
        XMFLOAT3 bt1;
        XMFLOAT3 bt2;

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