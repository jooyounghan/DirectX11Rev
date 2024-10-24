#include "AssetPriorityManager.h"
#include "AAssetFile.h"
#include "BaseMeshAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "MapAsset.h"
#include "MaterialAsset.h"
#include "BaseTextureAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

using namespace std;

PriorityNode::PriorityNode(const string& AssetTypeIn) : AssetType(AssetTypeIn) {}

void PriorityNode::AddPrerequisite(shared_ptr<PriorityNode> NodeIn) 
{ 
	Prerequisites.push_back(NodeIn);
}

AssetPriorityManager::AssetPriorityManager()
{
	// Mesh
	shared_ptr<PriorityNode> BaseMesh = make_shared<PriorityNode>(BaseMeshAsset::BaseMeshAssetKind);
	shared_ptr<PriorityNode> StaticMesh = make_shared<PriorityNode>(StaticMeshAsset::StaticMeshAssetKind);
	shared_ptr<PriorityNode> SkeletalMesh = make_shared<PriorityNode>(SkeletalMeshAsset::SkeletalMeshAssetKind);
	shared_ptr<PriorityNode> Bone = make_shared<PriorityNode>(BoneAsset::BoneAssetKind);
	shared_ptr<PriorityNode> Map = make_shared<PriorityNode>(MapAsset::MapAssetKind);
	shared_ptr<PriorityNode> Material = make_shared<PriorityNode>(MaterialAsset::MaterialAssetKind);
	shared_ptr<PriorityNode> BaseTexture = make_shared<PriorityNode>(BaseTextureAsset::BaseTextureAssetKind);
	shared_ptr<PriorityNode> EXRTexture = make_shared<PriorityNode>(EXRTextureAsset::EXRTextureAssetKind);
	shared_ptr<PriorityNode> DDSTexture = make_shared<PriorityNode>(DDSTextureAsset::DDSTextureAssetKind);

	PriorityNodes.emplace_back(BaseMesh);
	PriorityNodes.emplace_back(StaticMesh);
	PriorityNodes.emplace_back(SkeletalMesh);
	PriorityNodes.emplace_back(Bone);
	PriorityNodes.emplace_back(Map);
	PriorityNodes.emplace_back(Material);
	PriorityNodes.emplace_back(BaseTexture);
	PriorityNodes.emplace_back(EXRTexture);
	PriorityNodes.emplace_back(DDSTexture);

	Material->AddPrerequisite(BaseTexture);
	Material->AddPrerequisite(EXRTexture);
	Material->AddPrerequisite(DDSTexture);

	StaticMesh->AddPrerequisite(Material);

	SkeletalMesh->AddPrerequisite(Bone);
	SkeletalMesh->AddPrerequisite(Material);
	StaticMesh->AddPrerequisite(Material);

	Map->AddPrerequisite(BaseMesh);
	Map->AddPrerequisite(StaticMesh);
	Map->AddPrerequisite(SkeletalMesh);
}

unordered_map<string, size_t> AssetPriorityManager::GetAssetLoadPriorities()
{
	unordered_map<string, size_t> PriorityResults;
	ReadyTopologySort();
	TopologySort(PriorityResults);
	return PriorityResults;
}

void AssetPriorityManager::ReadyTopologySort()
{
	for (auto& node : PriorityNodes)
	{
		node->SetPrerequisitiesCount(node->GetPrerequisites().size());
		node->SetIsSorted(false);
	}
}

void AssetPriorityManager::TopologySort(
	unordered_map<string, size_t>& Result
)
{
	static size_t priority = 0;

	bool IsContinue = false;
	vector<shared_ptr<PriorityNode>> NodesForDisconnect;

	for (auto& node : PriorityNodes)
	{
		if (!node->GetIsSorted() && node->GetPrerequisitiesCount() == 0)
		{
			Result.emplace(node->GetAssetType(), priority);
			node->SetIsSorted(true);
			NodesForDisconnect.push_back(node);
			priority++;
			IsContinue = true;
		}
	}

	for (auto& node : PriorityNodes)
	{
		if (!node->GetIsSorted())
		{
			const vector<shared_ptr<PriorityNode>>& Prerequisites = node->GetPrerequisites();
			for (auto& NodeForDisconnect : NodesForDisconnect)
			{
				for (auto& PrerequisiteNode : Prerequisites)
				{
					if (NodeForDisconnect.get() == PrerequisiteNode.get())
					{
						node->SetPrerequisitiesCount(node->GetPrerequisitiesCount() - 1);
					}
				}
			}
		}
	}

	if (IsContinue) TopologySort(Result);
}

