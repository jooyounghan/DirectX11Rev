#include "ASphereCollisionComponent.h"

using namespace std;
using namespace DirectX;

ARenderSphereCollisionComponent::ARenderSphereCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const DirectX::XMFLOAT3& localPosition, 
	const float& radius
)
	: AComponent(componentName, componentID, localPosition, XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(1.f, 1.f, 1.f))
{
	SetBoundingProperties(localPosition, radius);
}

void ARenderSphereCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
