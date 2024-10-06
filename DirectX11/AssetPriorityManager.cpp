#include "AssetPriorityManager.h"
#include "AAssetFile.h"

using namespace std;

PriorityNode::PriorityNode(EAssetType AssetTypeIn) : AssetType(AssetTypeIn) {}

void PriorityNode::AddPrerequisite(shared_ptr<PriorityNode> NodeIn) 
{ 
	Prerequisites.push_back(NodeIn);
}

AssetPriorityManager::AssetPriorityManager()
{
	// Mesh
	shared_ptr<PriorityNode> BaseMesh = make_shared<PriorityNode>(EAssetType::BaseMesh);
	shared_ptr<PriorityNode> StaticMesh = make_shared<PriorityNode>(EAssetType::StaticMesh);
	shared_ptr<PriorityNode> SkeletalMesh = make_shared<PriorityNode>(EAssetType::SkeletalMesh);
	shared_ptr<PriorityNode> Bone = make_shared<PriorityNode>(EAssetType::Bone);
	shared_ptr<PriorityNode> Map = make_shared<PriorityNode>(EAssetType::Map);
	shared_ptr<PriorityNode> Material = make_shared<PriorityNode>(EAssetType::Material);
	shared_ptr<PriorityNode> BasicTexture = make_shared<PriorityNode>(EAssetType::BasicTexture);
	shared_ptr<PriorityNode> EXRTexture = make_shared<PriorityNode>(EAssetType::EXRTexture);
	shared_ptr<PriorityNode> DDSTexture = make_shared<PriorityNode>(EAssetType::DDSTexture);
	shared_ptr<PriorityNode> Animation = make_shared<PriorityNode>(EAssetType::Animation);

	PriorityNodes.emplace_back(BaseMesh);
	PriorityNodes.emplace_back(StaticMesh);
	PriorityNodes.emplace_back(SkeletalMesh);
	PriorityNodes.emplace_back(Bone);
	PriorityNodes.emplace_back(Map);
	PriorityNodes.emplace_back(Material);
	PriorityNodes.emplace_back(BasicTexture);
	PriorityNodes.emplace_back(EXRTexture);
	PriorityNodes.emplace_back(DDSTexture);
	PriorityNodes.emplace_back(Animation);

	Material->AddPrerequisite(BasicTexture);
	Material->AddPrerequisite(EXRTexture);
	Material->AddPrerequisite(DDSTexture);

	StaticMesh->AddPrerequisite(Material);

	SkeletalMesh->AddPrerequisite(Bone);
	SkeletalMesh->AddPrerequisite(Material);

	Animation->AddPrerequisite(SkeletalMesh);


	Map->AddPrerequisite(BaseMesh);
	Map->AddPrerequisite(StaticMesh);
	Map->AddPrerequisite(SkeletalMesh);
	Map->AddPrerequisite(Animation);
}

unordered_map<EAssetType, size_t> AssetPriorityManager::GetAssetLoadPriorities()
{
	unordered_map<EAssetType, size_t> PriorityResults;
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
	unordered_map<EAssetType, size_t>& Result
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

