#pragma once
#include "BoundingVolumeNode.h"

#include <unordered_map>

class ACollisionVisitor;
class ASceneRenderer;

class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy(const float& margin);

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
	void InsertCollidable(ID3D11Device* device, ICollisionAcceptor* collidable);
	void RemoveCollidable(ICollisionAcceptor* collidable);
	void UpdateCollidable(ID3D11Device* device, ICollisionAcceptor* collidable);
	void RenderCollidable(ASceneRenderer* sceneRenderer);
	void UpdateTransformation(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

public:
	void Traverse(ICollisionAcceptor* collidable);

private:
	void Refit(BoundingVolumeNode* node);
	void Rotate(BoundingVolumeNode* node);
	void TraverseImpl(BoundingVolumeNode* node, ICollisionAcceptor*& collidable, ACollisionVisitor& containVisitor);
	void UdpateTraverseImpl(BoundingVolumeNode* node, ID3D11DeviceContext* deviceContext);
	void RenderTraverseImpl(BoundingVolumeNode* node, ASceneRenderer* sceneRenderer);
};

