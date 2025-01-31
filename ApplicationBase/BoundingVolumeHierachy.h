#pragma once
#include "BoundingVolumeNode.h"

#include <unordered_map>

class ACollisionVisitor;

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

private:
	size_t m_searchCount = 0;

public:
	inline const size_t& GetSerachCount() { return m_searchCount; }
	void ResetSerachCount() { m_searchCount = 0; }

public:
	void InsertCollidable(ICollisionAcceptor* collidable);
	void RemoveCollidable(ICollisionAcceptor* collidable);
	void UpdateCollidable(ICollisionAcceptor* collidable);

public:
	void Traverse(ICollisionAcceptor* collidable);

private:
	void Refit(BoundingVolumeNode* node);
	void Rotate(BoundingVolumeNode* node);
	void TraverseImpl(BoundingVolumeNode* node, ICollisionAcceptor*& collidable, ACollisionVisitor& containVisitor);
};

