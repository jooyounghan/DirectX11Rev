#include "RenderControlOption.h"
#include "AComponent.h"

BoundingVolumeHierarchy RenderControlOption::RenderBVH(30.f);

void RenderControlOption::InsertBVHImpl(ID3D11Device* device, ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVH.InsertCollidable(device, acceptor);
}

void RenderControlOption::RemoveBVHImpl(ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVH.RemoveCollidable(acceptor);
}

void RenderControlOption::UpdateBVHImpl(ID3D11Device* device, ICollisionAcceptor* accpetor)
{
	RenderControlOption::RenderBVH.UpdateCollidable(device, accpetor);
}

void RenderControlOption::OnCollideImpl(AComponent* component)
{
	component->SetDebugColor(DirectX::XMFLOAT3(0.f, 1.f, 0.f));
	component->SetIsModified(true);
	component->SetRenderable(true);
}
