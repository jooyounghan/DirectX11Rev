#include "AOrientedBoxCollisionComponent.h"

using namespace std;
using namespace DirectX;

ARenderOrientedBoxCollisionComponent::ARenderOrientedBoxCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const XMFLOAT3& localPosition, 
	const XMFLOAT3& localAngle, 
	const XMFLOAT3& extension
)
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f))
{
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(localAngle.x, localAngle.y, localAngle.z);
	XMFLOAT4 orientation;
	XMStoreFloat4(&orientation, rotationQuaternion);
	SetBoundingProperties(localPosition, extension, orientation);
}

void ARenderOrientedBoxCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
