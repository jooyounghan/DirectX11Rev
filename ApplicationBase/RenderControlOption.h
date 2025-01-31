#pragma once
#include "ICollisionOption.h"
#include "BoundingVolumeHierarchy.h"

class RenderControlOption : public ICollisionOption
{
public:
	static BoundingVolumeHierarchy RenderBVH;

public:
	virtual void InsertBVHImpl(ID3D11Device* device, ICollisionAcceptor* acceptor) override;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) override;
	virtual void UpdateBVHImpl(ID3D11Device* device, ICollisionAcceptor* accpetor) override;
	virtual void OnCollideImpl(AComponent* component) override;
};

