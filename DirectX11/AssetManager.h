#pragma once
#include "Debugable.h"

#include "DirectXMath.h"

#include <string>
#include <unordered_map>
#include <list>
#include <memory>
#include <stack>

struct ID3D11Device;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiString;

enum class EAssetType;

class AAssetFile;

class AMeshAsset;
class BaseMeshAsset;
class ANBTMeshAsset;
class StaticMeshAsset;
class SkeletalMeshAsset;
class BoneAsset;

class MapAsset;

class NormalTextureAsset;
class EXRTextureAsset;
class DDSTextureAsset;

enum class EFileType
{
	ModelFile,
	NormalTextureFile,
	EXRTextureFile,
	DDSTextureFile,
};

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

public:
	void LoadAssetFile(const std::string& AssetPathIn);

private:
	template<typename T, typename ...Args>
	void LoadAssetFileHelper(
		FILE* FileIn,
		std::unordered_map<std::string, std::shared_ptr<T>>& ManagingContainer,
		const std::string& AssetName,
		Args... AssetConstructArgs
	);

public:
	void LoadFile(const std::string& FilePathIn);

private:
	static std::unordered_map<std::string, EFileType> FileExtensionToType;

private:
	void LoadModelFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadNormalTextureFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadEXRTextureFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadDDSTextureFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);

private:
	void LoadMesh(bool IsGltf, const std::string AssetName, const aiScene* const Scene);
	void LoadMaterial(const std::string AssetName, const aiScene* const Scene);
	void LoadAnimation(const std::string AssetName, const aiScene* const Scene);

private:
	bool HasBone(const aiScene* const Scene);

private:
	std::unordered_map<std::string, std::shared_ptr<MapAsset>> ManagingMaps;
	std::unordered_map<std::string, std::shared_ptr<BoneAsset>> ManagingBones;
	std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>> ManagingStaticMeshes;
	std::unordered_map<std::string, std::shared_ptr<SkeletalMeshAsset>> ManagingSkeletalMeshes;
	std::unordered_map<std::string, std::shared_ptr<NormalTextureAsset>> ManagingNormalTextures;
	std::unordered_map<std::string, std::shared_ptr<EXRTextureAsset>> ManagingEXRTextures;
	std::unordered_map<std::string, std::shared_ptr<DDSTextureAsset>> ManagingDDSTextures;

	MakeGetter(ManagingMaps);
	MakeGetter(ManagingBones);
	MakeGetter(ManagingStaticMeshes);
	MakeGetter(ManagingSkeletalMeshes);
	MakeGetter(ManagingNormalTextures);
	MakeGetter(ManagingEXRTextures);
	MakeGetter(ManagingDDSTextures);

private:
	std::unordered_map<std::string, BaseMeshAsset*> ManagingBaseMeshes;
	MakeGetter(ManagingBaseMeshes);


private:
	template<typename T>
	void AddToManagingContainer(
		std::unordered_map<std::string, T>& ManagingContainer,
		T& AddedAsset
	);

private:
	std::unordered_map<std::string, std::list<std::string>> FileNameToAssetNames;

private:
	std::stack<std::string> FileNameStack;

public:
	AAssetFile* GetManagingAsset(const std::string& AssetNameIn);

public:
	std::shared_ptr<MapAsset> GetManagingMap(const std::string MapAssetName);
	std::shared_ptr<BoneAsset> GetManagingBone(const std::string MapAssetName);
	std::shared_ptr<StaticMeshAsset> GetManagingStaticMesh(const std::string MapAssetName);
	std::shared_ptr<SkeletalMeshAsset> GetManagingSkeletalMesh(const std::string MapAssetName);
	std::shared_ptr<NormalTextureAsset> GetManagingNormalTexture(const std::string MapAssetName);
	std::shared_ptr<EXRTextureAsset> GetManagingEXRTexture(const std::string MapAssetName);
	std::shared_ptr<DDSTextureAsset> GetManagingDDSTexture(const std::string MapAssetName);
	BaseMeshAsset* GetManagingBaseMesh(const std::string MapAssetName);

private:
	template<typename T> 
	static T GetManagingAssetHelper(
		std::unordered_map<std::string, T>& ManagingContainer, 
		const std::string AssetName
	);

private:
	void ProcessNodeForMesh(
		bool IsGltf,
		const aiScene* const Scene, 
		const aiNode* const Node,
		StaticMeshAsset* StaticMesh,
		const DirectX::XMMATRIX& ParentMatrix
	);
	void ProcessNodeForMesh(
		bool IsGltf,
		const aiScene* const Scene, 
		const aiNode* const Node,
		SkeletalMeshAsset* SkeletalMesh,
		BoneAsset* BoneA,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	void ProcessNodeForBone(
		const aiScene* const Scene,
		const aiNode* const Node,
		BoneAsset* Bone
	);

private:
	void RestructBaseVertices(const unsigned int& NumVertices, ANBTMeshAsset* MeshAssetIn, const size_t LodLevel);
	size_t GetTotalLODCountFromScene(const aiScene* const Scene);
	size_t GetLODLevelFromMeshName(const aiString& MeshName);


private:
	void LoadMeshElement(
		bool IsGltf,
		const aiMesh* const Mesh,
		StaticMeshAsset* StaticMesh,
		const size_t LodLevel,
		const DirectX::XMMATRIX& ParentMatrix
	);

	void LoadMeshElement(
		bool IsGltf,
		const aiMesh* const Mesh,
		SkeletalMeshAsset* SkeletalMesh,
		const size_t LodLevel,
		BoneAsset* Bone,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	void LoadPosition(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		std::vector<XMFLOAT3>& Postions,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	void LoadBone(
		const aiScene* const Scene,
		BoneAsset* BoneAsset
	);

private:
	void LoadTextureCoord(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		std::vector<XMFLOAT2>& UVTextures
	);

private:
	void LoadBlendWeightAndIndex(
		const aiMesh* const Mesh,
		std::vector<XMFLOAT4>& BlendWeight,
		std::vector<XMINT4>& BlendIndex
	);

private:
	void LoadIndices(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		std::vector<uint32_t>& IndicesIn
	);

private:
	template<typename T>
	void LoadTBN(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshObjectInstance,
		const size_t& LodLevel
	);

	template<typename T>
	void LoadTBNAsGltf(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshObjectInstance,
		const size_t& LodLevel
	);

private:
	template <typename T>
	void CalculateTB(
		const aiMesh* const Mesh,
		size_t IndexStartIdx,
		T* MeshObjectInstance,
		const size_t& LodLevel
	);

private:
	void PreloadAssets();
};

