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

ACollisionComponent::~ACollisionComponent()
{
	if (m_collisionOption)
	{
		delete m_collisionOption;
	}
}

void ACollisionComponent::SetCollisionOption(ID3D11Device* device, ICollisionOption* collisionOption)
{ 
	if (m_collisionOption) delete m_collisionOption;
	m_collisionOption = collisionOption; 
}
