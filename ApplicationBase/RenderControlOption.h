#pragma once
#include "ICollisionOption.h"
#include "BoundingVolumeHierarchy.h"

class RenderControlOption : public ICollisionOption
{
public:
	static BoundingVolumeHierarchy RenderBVH;

public:
	virtual void InsertBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void UpdateBVHImpl(ICollisionAcceptor* accpetor) override;
	virtual void OnCollideImpl(AComponent* component) override;
};

