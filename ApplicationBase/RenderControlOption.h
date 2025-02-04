#pragma once
#include "ICollisionOption.h"
#include "BoundingVolumeHierarchy.h"

class ComponentCollector;

class RenderControlOption : public ICollisionOption
{
public:
	static BoundingVolumeHierarchy RenderBVH;

public:
	virtual void InsertBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void UpdateBVHImpl(ICollisionAcceptor* accpetor) override;
	virtual void OnCollideImpl(AComponent* component) override;

public:
	static std::vector<AComponent*> GetRenderableComponents(
		ICollisionAcceptor* renderControlOptionCollidable, 
		const std::vector<AComponent*>& components
	);
	static  void CollectRenderableComponentsImpl(
		const std::vector<AComponent*>& components,
		ComponentCollector& renderableComponentCollector
	);
};

