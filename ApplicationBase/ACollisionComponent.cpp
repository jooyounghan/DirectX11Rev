#include "ACollisionComponent.h"
#include "ICollisionOption.h"

using namespace std;
using namespace DirectX;

ACollisionComponent::ACollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle
)
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f))
{
}

void ACollisionComponent::SetCollisionOption(ICollisionOption* collisionOption) 
{ 
	m_collisionOption = collisionOption; 
}
