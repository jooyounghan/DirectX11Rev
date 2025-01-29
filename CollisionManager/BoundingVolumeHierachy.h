#pragma once
#include "BoundingVolumeNode.h"

#include <unordered_map>

class BoundingVolumeHierachy
{
public:
	BoundingVolumeHierachy(const float& margin);

private:
	BoundingVolumeNode* m_rootNode = nullptr;

private:
	std::unordered_map<ICollisionAcceptor*, BoundingVolumeNode*> m_collidablesToNode;

private:
	const float m_margin;

public:
	void InsertCollidable(ICollisionAcceptor* collidable);
	void RemoveCollidable(ICollisionAcceptor* collidable);
	void UpdateCollidable(ICollisionAcceptor* collidable);

private:
	void RestructVolume(BoundingVolumeNode* node);

};

