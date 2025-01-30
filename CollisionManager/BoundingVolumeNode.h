#pragma once
#include "ICollisionAcceptor.h"
#include <vector>

class BoundingVolumeNode
{
public:
	BoundingVolumeNode(
		const DirectX::BoundingBox& boundingBox, 
		ICollisionAcceptor* const collidable = nullptr
	);
	BoundingVolumeNode(
		const DirectX::XMFLOAT3& lowerBound, 
		const DirectX::XMFLOAT3& upperBound, 
		ICollisionAcceptor* const collidable = nullptr
	);

private:
	DirectX::BoundingBox m_box;
	DirectX::XMFLOAT3 m_lowerBound;
	DirectX::XMFLOAT3 m_upperBound;

public:
	BoundingVolumeNode* m_parentNode = nullptr;
	float m_volumeSize = 0.f;

public:
	BoundingVolumeNode* m_left;
	BoundingVolumeNode* m_right;

public:
	ICollisionAcceptor* m_collidable = nullptr;

public:
	inline const DirectX::BoundingBox& GetBoundingBox() const { return m_box; }
	inline void SetBoundingBox(const DirectX::BoundingBox& box) { m_box = box; }

	inline const float GetVolumeSize() const 
	{ 
		return (m_box.Extents.x * 2) * (m_box.Extents.y * 2) * (m_box.Extents.z * 2); 
	}
	inline const bool IsLeaf() const { return m_collidable != nullptr; }

public:
	static DirectX::BoundingBox CreateUnionBoundingBox(
		const BoundingVolumeNode* const boundingVolume1,
		const BoundingVolumeNode* const boundingVolume2,
		const float& margin
	);

	static const float GetUnionBoundingVolumeSize(
		const BoundingVolumeNode* boundingVolume1,
		const BoundingVolumeNode* boundingVolume2,
		const float& margin
	);

	static void FindBestLeafNode(
		BoundingVolumeNode* const targetNode,
		const BoundingVolumeNode* const insertNode,
		const float& margin,
		float& minVolumeSize,
		BoundingVolumeNode*& bestSiblingLeafNode
	);
	static void GetBounds(
		const BoundingVolumeNode* const boundingVolume1,
		const BoundingVolumeNode* const boundingVolume2,
		const float& margin,
		DirectX::XMFLOAT3& lowerBoundOut,
		DirectX::XMFLOAT3& upperBoundOut
	);
	static DirectX::BoundingBox CreateBoundingBox(
		const DirectX::XMFLOAT3& lowerBound,
		const DirectX::XMFLOAT3& upperBound
	);
	static void GetSiblingNode(
		BoundingVolumeNode* node, BoundingVolumeNode* siblingOut, bool& isSiblingLeft
	);
};

