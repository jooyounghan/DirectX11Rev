#include "RenderControlOption.h"
#include "AComponent.h"

BoundingVolumeHierarchy RenderControlOption::RenderBVH(10.f);

void RenderControlOption::InsertBVHImpl(ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVH.InsertCollidable(acceptor);
}

void RenderControlOption::RemoveBVHImpl(ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVH.RemoveCollidable(acceptor);
}

void RenderControlOption::UpdateBVHImpl(ICollisionAcceptor* accpetor)
{
	RenderControlOption::RenderBVH.UpdateCollidable(accpetor);
}

void RenderControlOption::OnCollideImpl(AComponent* component)
{
	component->SetIsModified(true);
	component->SetRenderable(true);
}
