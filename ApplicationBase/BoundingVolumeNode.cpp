
#include "BoundingVolumeNode.h"

using namespace std;
using namespace DirectX;

BoundingVolumeNode::BoundingVolumeNode(
	const BoundingBox& boundingBox, 
	ICollisionAcceptor* const collidable
)
	: 
	m_box(boundingBox), m_collidable(collidable)
{
	m_volumeSize = GetVolumeSize();
}

DirectX::BoundingBox BoundingVolumeNode::CreateUnionBoundingBox(
	const BoundingVolumeNode* const boundingVolume1,
	const BoundingVolumeNode* const boundingVolume2,
	const float& margin
)
{
	XMFLOAT3 lowerBound;
	XMFLOAT3 upperBound;
	GetBounds(boundingVolume1, boundingVolume2, margin, lowerBound, upperBound);

	return CreateBoundingBox(lowerBound, upperBound);
}

const float BoundingVolumeNode::GetUnionBoundingVolumeSize(
	const BoundingVolumeNode* const boundingVolume1,
	const BoundingVolumeNode* const boundingVolume2,
	const float& margin
)
{
	XMFLOAT3 lowerBound;
	XMFLOAT3 upperBound;
	GetBounds(boundingVolume1, boundingVolume2, margin, lowerBound, upperBound);

	return (upperBound.x - lowerBound.x) * (upperBound.y - lowerBound.y) * (upperBound.z - lowerBound.z);
}

void BoundingVolumeNode::GetBounds(
	const BoundingVolumeNode* const boundingVolume1,
	const BoundingVolumeNode* const boundingVolume2,
	const float& margin,
	XMFLOAT3& lowerBoundOut, 
	XMFLOAT3& upperBoundOut
)
{

	lowerBoundOut.x = std::min(boundingVolume1->m_box.Center.x - boundingVolume1->m_box.Extents.x, boundingVolume2->m_box.Center.x - boundingVolume2->m_box.Extents.x);
	lowerBoundOut.y = std::min(boundingVolume1->m_box.Center.y - boundingVolume1->m_box.Extents.y, boundingVolume2->m_box.Center.y - boundingVolume2->m_box.Extents.y);
	lowerBoundOut.z = std::min(boundingVolume1->m_box.Center.z - boundingVolume1->m_box.Extents.z, boundingVolume2->m_box.Center.z - boundingVolume2->m_box.Extents.z);

	upperBoundOut.x = std::max(boundingVolume1->m_box.Center.x + boundingVolume1->m_box.Extents.x, boundingVolume2->m_box.Center.x + boundingVolume2->m_box.Extents.x);
	upperBoundOut.y = std::max(boundingVolume1->m_box.Center.y + boundingVolume1->m_box.Extents.y, boundingVolume2->m_box.Center.y + boundingVolume2->m_box.Extents.y);
	upperBoundOut.z = std::max(boundingVolume1->m_box.Center.z + boundingVolume1->m_box.Extents.z, boundingVolume2->m_box.Center.z + boundingVolume2->m_box.Extents.z);

	lowerBoundOut.x -= margin;
	lowerBoundOut.y -= margin;
	lowerBoundOut.z -= margin;

	upperBoundOut.x += margin;
	upperBoundOut.y += margin;
	upperBoundOut.z += margin;
}

DirectX::BoundingBox BoundingVolumeNode::CreateBoundingBox(
	const DirectX::XMFLOAT3& lowerBound, 
	const DirectX::XMFLOAT3& upperBound
)
{
	XMVECTOR lowerBoundV = XMLoadFloat3(&lowerBound);
	XMVECTOR upperBoundV = XMLoadFloat3(&upperBound);

	XMVECTOR centerV = (lowerBoundV + upperBoundV) / 2.f;
	XMVECTOR extentsV = (upperBoundV - lowerBoundV) / 2.f;

	XMFLOAT3 center, extents;
	XMStoreFloat3(&center, centerV);
	XMStoreFloat3(&extents, extentsV);

	return DirectX::BoundingBox(center, extents);
}

void BoundingVolumeNode::GetSiblingNode(BoundingVolumeNode* node, BoundingVolumeNode*& siblingOut, bool& isSiblingLeft)
{
	siblingOut = nullptr;
	BoundingVolumeNode* parentNode = node->m_parentNode;
	if (parentNode)
	{
		isSiblingLeft = parentNode->m_right == node;
		siblingOut = isSiblingLeft ? parentNode->m_left : parentNode->m_right;
	}
}

void BoundingVolumeNode::FindBestLeafNode(
	BoundingVolumeNode* const targetNode,
	const BoundingVolumeNode* const insertNode,
	const float& margin,
	float& minVolumeSize,
	BoundingVolumeNode*& bestSiblingLeafNode
)
{
	if (targetNode != nullptr)
	{
		const float volumeSize = GetUnionBoundingVolumeSize(targetNode, insertNode, margin);
		if (minVolumeSize > volumeSize)
		{
			minVolumeSize = volumeSize;
			if (targetNode->IsLeaf())
			{
				bestSiblingLeafNode = targetNode;
			}
			else
			{
				FindBestLeafNode(targetNode->m_left, insertNode, margin, minVolumeSize, bestSiblingLeafNode);
				FindBestLeafNode(targetNode->m_right, insertNode, margin, minVolumeSize, bestSiblingLeafNode);
			}
		}
	}

}
