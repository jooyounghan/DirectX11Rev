#pragma once
#include "HeaderHelper.h"

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

enum class EAssetType;

class PriorityNode
{
public:
	PriorityNode(EAssetType AssetTypeIn);

protected:
	EAssetType AssetType;
	std::vector<std::shared_ptr<PriorityNode>> Prerequisites;
	size_t PrerequisitiesCount = 0;
	bool IsSorted = false;

public:
	MakeGetter(AssetType);
	MakeGetter(Prerequisites);
	MakeSetterGetter(PrerequisitiesCount);
	MakeSetterGetter(IsSorted);

public:
	void AddPrerequisite(std::shared_ptr<PriorityNode> NodeIn);
};

class AssetPriorityManager
{
public:
	AssetPriorityManager();

private:
	std::list<std::shared_ptr<PriorityNode>> PriorityNodes;

public:
	std::unordered_map<EAssetType, size_t> GetAssetLoadPriorities();
	void ReadyTopologySort();
	void TopologySort(std::unordered_map<EAssetType, size_t>& Result);
};

