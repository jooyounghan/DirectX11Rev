#pragma once
#include "HeaderHelper.h"

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class PriorityNode
{
public:
	PriorityNode(const std::string& AssetTypeIn);

protected:
	std::string AssetType;
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
	std::unordered_map<std::string, size_t> GetAssetLoadPriorities();
	void ReadyTopologySort();
	void TopologySort(std::unordered_map<std::string, size_t>& Result);
};

