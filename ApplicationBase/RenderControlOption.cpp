#include "RenderControlOption.h"
#include "AComponent.h"
#include "ComponentCollector.h"

using namespace std;

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
	component->SetRenderable(true);
}

vector<AComponent*> RenderControlOption::GetRenderableComponents(
	ICollisionAcceptor* renderControlOptionCollidable, 
	const vector<AComponent*>& components
)
{
	vector<AComponent*> result;
	RenderBVH.Traverse(renderControlOptionCollidable);

	ComponentCollector renderableCollector;
	CollectRenderableComponentsImpl(components, renderableCollector);

	return renderableCollector.GetCollectedComponents();
}

void RenderControlOption::CollectRenderableComponentsImpl(
	const vector<AComponent*>& components,
	ComponentCollector& renderableComponentCollector
)
{
	for (AComponent* component : components)
	{
		if (component->IsRenderable())
		{
			component->Accept(&renderableComponentCollector);
			component->SetRenderable(component->GetDefaultRenderable());
			CollectRenderableComponentsImpl(component->GetChildComponents(), renderableComponentCollector);
		}
	}
}