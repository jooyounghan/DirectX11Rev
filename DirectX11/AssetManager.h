#pragma once
#include "Debugable.h"
#include "Delegation.h"
#include "AssetPriorityManager.h"

#include "directxmath/DirectXMath.h"

#include <unordered_map>
#include <filesystem>
#include <string>
#include <memory>
#include <stack>

struct ID3D11Device;
struct aiScene;
struct aiNode;
struct aiMesh;
struct aiString;
struct aiMaterial;
struct aiAnimation;
struct aiNodeAnim;

enum aiTextureType;
class AAssetFile;

class AMeshAsset;
class BaseMeshAsset;
class ANBTMeshAsset;
class StaticMeshAsset;
class SkeletalMeshAsset;
class BoneAsset;
class AnimationAsset;
class AnimationChannel;

class MapAsset;

class BaseTextureAsset;
class EXRTextureAsset;
class DDSTextureAsset;
class MaterialAsset;

enum class EFileType
{
	ModelFile,
	BasicTextureFile,
	EXRTextureFile,
	DDSTextureFile,
};

struct SAssetPreloadArgs
{
	std::string AssetName;
	std::string AssetPath;
	long LastReadPoint;
	std::string AssetType;
};

typedef std::function<void()> AssetAddedHandler;

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

public:
	Delegation<> AssetChangedEvent;

public:
	void LoadAssetFromFile(const std::string& FilePathIn);

private:
	static std::unordered_map<std::string, EFileType> FileExtensionToType;
	
private:
	AssetPriorityManager AssetPriorityManagerInstance;

private:
	void LoadModelAssetFromFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadBasicTextureAssetFromFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadEXRTextureAssetFromFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);
	void LoadDDSTextureAssetFromFile(const std::string& FilePathIn, const std::string& FileNameIn, const std::string& FileExtensionIn);

private:
	void LoadMeshAssetFromFile(bool IsGltf, const std::string& AssetName, const aiScene* const Scene);
	void LoadMaterialAssetFromFile(const std::string& FilePath, const std::string& AssetName, const aiScene* const Scene);
	std::shared_ptr<BaseTextureAsset> LoadBasicTextureFromMaterial(const aiScene* const Scene, aiMaterial* MaterialIn, aiTextureType TextureTypeIn);

private:
	void LoadAnimationAssetFromFile(const std::string& AssetName, const aiScene* const Scene);

private:
	bool HasBone(const aiScene* const Scene);

private:
	std::unordered_map<std::string, std::shared_ptr<MapAsset>> ManagingMaps;
	std::unordered_map<std::string, std::shared_ptr<BoneAsset>> ManagingBones;
	std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>> ManagingStaticMeshes;
	std::unordered_map<std::string, std::shared_ptr<SkeletalMeshAsset>> ManagingSkeletalMeshes;
	std::unordered_map<std::string, std::shared_ptr<BaseTextureAsset>> ManagingBasicTextures;
	std::unordered_map<std::string, std::shared_ptr<EXRTextureAsset>> ManagingEXRTextures;
	std::unordered_map<std::string, std::shared_ptr<DDSTextureAsset>> ManagingDDSTextures;
	std::unordered_map<std::string, std::shared_ptr<MaterialAsset>> ManagingMaterials;
	std::unordered_map<std::string, std::shared_ptr<AnimationAsset>> ManagingAnimations;

	MakeGetter(ManagingMaps);
	MakeGetter(ManagingBones);
	MakeGetter(ManagingStaticMeshes);
	MakeGetter(ManagingSkeletalMeshes);
	MakeGetter(ManagingBasicTextures);
	MakeGetter(ManagingEXRTextures);
	MakeGetter(ManagingDDSTextures);
	MakeGetter(ManagingMaterials);
	MakeGetter(ManagingAnimations);

private:
	std::unordered_map<std::string, BaseMeshAsset*> ManagingBaseMeshes;
	MakeGetter(ManagingBaseMeshes);


private:
	template<typename T>
	void SerailizeAndAddToContainer(
		std::unordered_map<std::string, T>& ManagingContainer,
		T& AddedAsset
	);

public:
	void SerailizeAndAddToContainer(std::shared_ptr<MapAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingMaps, AssetIn); }
	void SerailizeAndAddToContainer(std::shared_ptr<BoneAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingBones, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<StaticMeshAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingStaticMeshes, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<SkeletalMeshAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingSkeletalMeshes, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<BaseTextureAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingBasicTextures, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<EXRTextureAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingEXRTextures, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<DDSTextureAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingDDSTextures, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<MaterialAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingMaterials, AssetIn); };
	void SerailizeAndAddToContainer(std::shared_ptr<AnimationAsset>& AssetIn) { SerailizeAndAddToContainer(ManagingAnimations, AssetIn); };

private:
	template<typename T>
	void SerializeModifiedAsset(std::unordered_map<std::string, T>& ManagingContainer);

private:
	std::unordered_map<std::string, std::list<std::string>> FileNameToAssetNames;

private:
	std::vector<std::shared_ptr<MaterialAsset>> CurrentModelsMaterials;

public:
	AAssetFile* GetManagingAsset(const std::string& AssetNameIn);

public:
	std::shared_ptr<MapAsset> GetManagingMap(const std::string AssetName);
	std::shared_ptr<BoneAsset> GetManagingBone(const std::string AssetName);
	std::shared_ptr<StaticMeshAsset> GetManagingStaticMesh(const std::string AssetName);
	std::shared_ptr<SkeletalMeshAsset> GetManagingSkeletalMesh(const std::string AssetName);
	std::shared_ptr<BaseTextureAsset> GetManagingBasicTexture(const std::string AssetName);
	std::shared_ptr<EXRTextureAsset> GetManagingEXRTexture(const std::string AssetName);
	std::shared_ptr<DDSTextureAsset> GetManagingDDSTexture(const std::string AssetName);
	std::shared_ptr<MaterialAsset> GetManagingMaterial(const std::string AssetName);
	std::shared_ptr<AnimationAsset> GetManagingAnimation(const std::string AssetName);

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
		const size_t& VertexStartIdx,
		std::vector<DirectX::XMFLOAT3>& Postions,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	void LoadBone(
		const aiScene* const Scene,
		BoneAsset* BoneAsset
	);

private:
	void LoadAnimationChannels(const aiAnimation* const Animation, AnimationAsset* AnimAsset);
	void LoadPositionKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel);
	void LoadQuaternionKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel);
	void LoadScaleKeys(const aiNodeAnim* const NodeChannel, AnimationChannel& AnimChannel);

private:
	void LoadTextureCoord(
		const aiMesh* const Mesh,
		const size_t& VertexStartIdx,
		std::vector<DirectX::XMFLOAT2>& UVTextures
	);

private:
	void LoadBlendWeightAndIndex(
		const aiMesh* const Mesh,
		const size_t& VertexStartIdx,
		std::vector<DirectX::XMFLOAT4>& BlendWeight,
		std::vector<DirectX::XMINT4>& BlendIndex
	);

private:
	void LoadIndices(
		const aiMesh* const Mesh,
		const size_t& VertexStartIdx,
		std::vector<uint32_t>& IndicesIn
	);

private:
	template<typename T>
	void LoadTBN(
		const aiMesh* const Mesh,
		const size_t& VertexStartIdx,
		T* MeshObjectInstance,
		const size_t& LodLevel
	);

	template<typename T>
	void LoadTBNAsGltf(
		const aiMesh* const Mesh,
		const size_t& VertexStartIdx,
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
	void TraverseDirectory(
		const std::filesystem::path& PathIn, 
		std::vector<std::string>& AssetFilePathsIn
	);

private:
	void LoadAssetWithTopologySorting(const std::vector<std::string>& AssetPathsIn);
	template<typename T, typename ...Args>
	void LoadAssetHelper(
		FILE* FileIn,
		std::unordered_map<std::string, std::shared_ptr<T>>& ManagingContainer,
		const std::string& AssetName,
		Args... AssetConstructArgs
	);

};

