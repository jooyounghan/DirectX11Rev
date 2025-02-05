#pragma once
#include "ICollisionOption.h"
#include "BoundingVolumeHierarchy.h"

class ComponentCollector;

class RenderControlOption : public ICollisionOption
{
public:
	static std::unordered_map<uint32_t, BoundingVolumeHierarchy> RenderBVHs;

public:
	RenderControlOption(const uint32_t& sceneID);

protected:
	const uint32_t m_sceneID;

public:
	virtual void InsertBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void UpdateBVHImpl(ICollisionAcceptor* accpetor) override;
	virtual void OnCollideImpl(AComponent* component) override;

public:
	static std::vector<AComponent*> GetRenderableComponents(
		const uint32_t& sceneID,
		ICollisionAcceptor* renderControlOptionCollidable, 
		const std::vector<AComponent*>& components
	);
	static  void CollectRenderableComponentsImpl(
		const std::vector<AComponent*>& components,
		ComponentCollector& renderableComponentCollector
	);
};

