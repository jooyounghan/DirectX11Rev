#pragma once
#include <string>
#include "DirectXMath.h"

#include <unordered_map>
#include <memory>
#include <deque>

struct aiScene;
struct aiNode;
struct aiMesh;

struct ID3D11Device;

class IAssetFile;
class StaticMeshAsset;
class SkeletalMeshAsset;
class BoneAsset;

class AssetManager
{
public:
	AssetManager(ID3D11Device* DeviceIn);
	~AssetManager();

protected:
	ID3D11Device* DeviceCached = nullptr;

public:
	void LoadAssetFile(const std::string& FilePathIn);
	void LoadModelFile(const std::string& FilePathIn);

private:
	void LoadMesh(bool IsGltf, const std::string AssetName, const aiScene* const Scene);
	void LoadMaterial(const std::string AssetName, const aiScene* const Scene);
	void LoadAnimation(const std::string AssetName, const aiScene* const Scene);

private:
	bool HasBone(const aiScene* const Scene);

private:
	std::deque<SkeletalMeshAsset*> WatingSkeletalMeshes;

public:
	void ProcessLinkSkeletalMeshAsset();

private:
	std::unordered_map<std::string, std::shared_ptr<IAssetFile>> ManagingAssets;

public:
	IAssetFile* GetAsset(const std::string AssetName);

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

