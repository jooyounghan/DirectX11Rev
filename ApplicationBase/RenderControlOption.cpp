#include "RenderControlOption.h"
#include "AComponent.h"
#include "ComponentCollector.h"

using namespace std;

std::unordered_map<uint32_t, BoundingVolumeHierarchy> RenderControlOption::RenderBVHs;

RenderControlOption::RenderControlOption(const uint32_t& sceneID)
	: m_sceneID(sceneID)
{
}

void RenderControlOption::InsertBVHImpl(ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVHs[m_sceneID].InsertCollidable(acceptor);
}

void RenderControlOption::RemoveBVHImpl(ICollisionAcceptor* acceptor)
{
	RenderControlOption::RenderBVHs[m_sceneID].RemoveCollidable(acceptor);
}

void RenderControlOption::UpdateBVHImpl(ICollisionAcceptor* accpetor)
{
	RenderControlOption::RenderBVHs[m_sceneID].UpdateCollidable(accpetor);
}

void RenderControlOption::OnCollideImpl(AComponent* component)
{
	component->SetRenderable(true);
}

vector<AComponent*> RenderControlOption::GetRenderableComponents(
	const uint32_t& sceneID,
	ICollisionAcceptor* renderControlOptionCollidable, 
	const vector<AComponent*>& components
)
{
	vector<AComponent*> result;
	RenderBVHs[sceneID].Traverse(renderControlOptionCollidable);

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