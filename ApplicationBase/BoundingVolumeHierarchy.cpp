#include "BoundingVolumeHierarchy.h"
#include "ContainVisitor.h"
#include "IntersectVisitor.h"

using namespace std;
using namespace DirectX;

BoundingVolumeHierarchy::BoundingVolumeHierarchy(const float& margin)
	: m_margin(margin)
{
}

void BoundingVolumeHierarchy::InsertCollidable(ICollisionAcceptor* collidable)
{
	BoundingVolumeNode* leafNode = new BoundingVolumeNode(collidable->GetBoundingBox(m_margin), collidable);
	m_collidablesToNode.emplace(collidable, leafNode);

	if (m_rootNode == nullptr)
	{
		m_rootNode = leafNode;
		return;
	}

	float minVolumeSize = FLT_MAX;
	BoundingVolumeNode* bestLeafNode = m_rootNode;
	BoundingVolumeNode::FindBestLeafNode(m_rootNode, leafNode, m_margin, minVolumeSize, bestLeafNode);

	BoundingBox newParentBox = BoundingVolumeNode::CreateUnionBoundingBox(leafNode, bestLeafNode, m_margin);
	BoundingVolumeNode* oldParent = bestLeafNode->m_parentNode;
	BoundingVolumeNode* newParent = new BoundingVolumeNode(newParentBox);

	if (oldParent != nullptr)
	{
		if (oldParent->m_left == bestLeafNode)
		{
			oldParent->m_left = newParent;
		}
		else
		{
			oldParent->m_right = newParent;
		}
		newParent->m_parentNode = oldParent;

		newParent->m_left = bestLeafNode;
		newParent->m_right = leafNode;
		bestLeafNode->m_parentNode = newParent;
		leafNode->m_parentNode = newParent;
		Refit(newParent);
	}
	else
	{
		newParent->m_left = bestLeafNode;
		newParent->m_right = leafNode;
		bestLeafNode->m_parentNode = newParent;
		leafNode->m_parentNode = newParent;
		m_rootNode = newParent;
	}
}

void BoundingVolumeHierarchy::RemoveCollidable(ICollisionAcceptor* collidable)
{
	if (m_collidablesToNode.find(collidable) != m_collidablesToNode.end())
	{
		BoundingVolumeNode* boundingVolumeNode = m_collidablesToNode[collidable];
		m_collidablesToNode.erase(collidable);

		BoundingVolumeNode* parentNode = boundingVolumeNode->m_parentNode;

		// Case 1 (No Parent)
		if (parentNode == nullptr)
		{
			delete boundingVolumeNode;
			m_rootNode = nullptr;
			return;
		}

		BoundingVolumeNode* grandParentNode = parentNode->m_parentNode;
		if (grandParentNode == nullptr)
		{
			// Case 2 (No GrandParent)
			if (parentNode->m_left == boundingVolumeNode)
			{
				m_rootNode = parentNode->m_right;
			}
			else
			{
				m_rootNode = parentNode->m_left;
			}
			m_rootNode->m_parentNode = nullptr;
		}
		else
		{
			// Case 3 (Has GrandParent)
			BoundingVolumeNode* siblingNode = nullptr;
			bool isSiblingLeft = false;
			BoundingVolumeNode::GetSiblingNode(boundingVolumeNode, siblingNode, isSiblingLeft);
			if (siblingNode)
			{
				if (grandParentNode->m_left == parentNode)
				{
					grandParentNode->m_left = siblingNode;
				}
				else
				{
					grandParentNode->m_right = siblingNode;
				}
				siblingNode->m_parentNode = grandParentNode;
				Refit(grandParentNode);
			}
		}
		delete boundingVolumeNode;
		delete parentNode;
	}
}

void BoundingVolumeHierarchy::UpdateCollidable(ICollisionAcceptor* collidable)
{
	if (m_collidablesToNode.find(collidable) != m_collidablesToNode.end())
	{
		BoundingVolumeNode* boundingVolumeNode = m_collidablesToNode[collidable];

		if (!collidable->IsIntersectBoundingBox(boundingVolumeNode->GetBoundingBox()))
		{
			RemoveCollidable(collidable);
			InsertCollidable(collidable);
		}
	}
}

void BoundingVolumeHierarchy::Traverse(ICollisionAcceptor* collidable)
{
	IntersectVisitor containVisitor(collidable);
	TraverseImpl(m_rootNode, collidable, containVisitor);
}

void BoundingVolumeHierarchy::Refit(BoundingVolumeNode* node)
{
	if (node == nullptr) return;
	node->SetBoundingBox(BoundingVolumeNode::CreateUnionBoundingBox(node->m_left, node->m_right, m_margin));
	Rotate(node);
	Refit(node->m_parentNode);
}

void BoundingVolumeHierarchy::Rotate(BoundingVolumeNode* node)
{
	BoundingVolumeNode* parent = node->m_parentNode;
	BoundingVolumeNode* leftChildNode = node->m_left;
	BoundingVolumeNode* rightChildNode = node->m_right;
	BoundingVolumeNode* siblingNode = nullptr;
	bool isSiblingLeft = false;
	BoundingVolumeNode::GetSiblingNode(node, siblingNode, isSiblingLeft);

	if (parent && leftChildNode && rightChildNode && siblingNode)
	{
		float currentBVSize = BoundingVolumeNode::GetUnionBoundingVolumeSize(leftChildNode, rightChildNode, m_margin);
		float rotationLeftWithSiblingSize = BoundingVolumeNode::GetUnionBoundingVolumeSize(siblingNode, rightChildNode, m_margin);
		float rotationRightWithSiblingSize = BoundingVolumeNode::GetUnionBoundingVolumeSize(siblingNode, leftChildNode, m_margin);

		bool rotateLeft = (rotationLeftWithSiblingSize < rotationRightWithSiblingSize);
		float bestRotationSize = rotateLeft ? rotationLeftWithSiblingSize : rotationRightWithSiblingSize;

		if (bestRotationSize < currentBVSize)
		{
			BoundingVolumeNode* replacingChild = rotateLeft ? node->m_left : node->m_right;

			if (isSiblingLeft)
			{
				parent->m_left = replacingChild;
			}
			else
			{
				parent->m_right = replacingChild;
			}
			replacingChild->m_parentNode = parent;


			if (rotateLeft)
			{
				node->m_left = siblingNode;
			}
			else
			{
				node->m_right = siblingNode;
			}
			siblingNode->m_parentNode = node;
		}

		node->SetBoundingBox(BoundingVolumeNode::CreateUnionBoundingBox(node->m_left, node->m_right, m_margin));
	}
}

void BoundingVolumeHierarchy::TraverseImpl(BoundingVolumeNode* node, ICollisionAcceptor*& collidable, ACollisionVisitor& containVisitor)
{
	if (node)
	{
		m_searchCount++;

		if (collidable->IsIntersectBoundingBox(node->GetBoundingBox()))
		{
			if (node->IsLeaf())
			{
				m_searchCount++;
				if (node->m_collidable->Accept(containVisitor))
				{
					node->m_collidable->OnCollide(collidable);
				}
			}
			TraverseImpl(node->m_left, collidable, containVisitor);
			TraverseImpl(node->m_right, collidable, containVisitor);
		}
	}
}
