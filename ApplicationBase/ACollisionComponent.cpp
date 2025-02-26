#include "ACollisionComponent.h"
#include "ICollisionOption.h"

using namespace std;
using namespace DirectX;

ACollisionComponent::ACollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle,
	const XMFLOAT3& scale
)
	: AComponent(componentName, componentID, localPosition, localAngle, scale)
{
}

ACollisionComponent::~ACollisionComponent()
{
	if (m_collisionOption)
	{
		delete m_collisionOption;
	}
}

void ACollisionComponent::SetCollisionOption(ICollisionOption* collisionOption)
{ 
	if (m_collisionOption) delete m_collisionOption;
	m_collisionOption = collisionOption; 
}
