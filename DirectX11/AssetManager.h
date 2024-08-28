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

class GraphicsPipeline;

enum class EAssetType;

class AAssetFile;
class AMeshAsset;
class StaticMeshAsset;
class SkeletalMeshAsset;
class BoneAsset;
class MapAsset;

class AssetManager
{
public:
	AssetManager(GraphicsPipeline* GraphicsPipelineInstance);
	~AssetManager();

protected:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;
	MakeGetter(GraphicsPipelineCached);

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
	void LoadModelFile(const std::string& FilePathIn);

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
	std::unordered_map<std::string, std::list<std::string>> FileNameToAssetNames;
	MakeGetter(ManagingMaps);
	MakeGetter(ManagingBones);
	MakeGetter(ManagingStaticMeshes);
	MakeGetter(ManagingSkeletalMeshes);

private:
	std::unordered_map<std::string, std::shared_ptr<AAssetFile>> ManagingAssets;
	
public:
	AAssetFile* GetManagingAsset(const std::string& AssetNameIn);

private:
	std::stack<std::string> FileNameStack;

public:
	std::shared_ptr<MapAsset> GetManagingMap(const std::string MapAssetName);
	std::shared_ptr<BoneAsset> GetManagingBone(const std::string MapAssetName);
	std::shared_ptr<StaticMeshAsset> GetManagingStaticMesh(const std::string MapAssetName);
	std::shared_ptr<SkeletalMeshAsset> GetManagingSkeletalMesh(const std::string MapAssetName);

private:
	template<typename T> 
	static std::shared_ptr<T> GetManagingAssetHelper(
		std::unordered_map<std::string, std::shared_ptr<T>>& ManagingContainer, 
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
	void RestructBaseVertices(const unsigned int& NumVertices, AMeshAsset* MeshAssetIn);

private:
	void LoadMeshElement(
		bool IsGltf,
		const aiMesh* const Mesh,
		StaticMeshAsset* StaticMesh,
		const DirectX::XMMATRIX& ParentMatrix
	);

	void LoadMeshElement(
		bool IsGltf,
		const aiMesh* const Mesh,
		SkeletalMeshAsset* SkeletalMesh,
		BoneAsset* Bone,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	template<typename T>
	void LoadPosition(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshAsset,
		const DirectX::XMMATRIX& ParentMatrix
	);

private:
	void LoadBone(
		const aiScene* const Scene,
		SkeletalMeshAsset* SkeletalMesh,
		BoneAsset* BoneAsset
	);

private:
	template<typename T>
	void LoadTextureCoord(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshAsset
	);

private:
	void LoadIndices(
		std::vector<uint32_t>& IndicesIn,
		const aiMesh* const Mesh
	);

private:
	template<typename T>
	void LoadTBN(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshAsset
	);

	template<typename T>
	void LoadTBNAsGltf(
		const aiMesh* const Mesh,
		size_t VertexStartIdx,
		T* MeshAsset
	);

private:
	template <typename T>
	void CalculateTB(
		const aiMesh* const Mesh,
		size_t IndexStartIdx,
		T* MeshAsset
	);

private:
	void PreloadAssets();
};

