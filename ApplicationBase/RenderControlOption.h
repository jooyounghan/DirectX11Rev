#pragma once
#include "ICollisionOption.h"
#include "BoundingVolumeHierachy.h"

class RenderControlOption : public ICollisionOption
{
public:
	static BoundingVolumeHierachy RenderBVH;

public:
	virtual void InsertBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void UpdateBVHImpl(ICollisionAcceptor* accpetor) override;
	virtual void OnCollideImpl(AComponent* component) override;
};

