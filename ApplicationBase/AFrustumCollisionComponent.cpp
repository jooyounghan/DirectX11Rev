#include "AFrustumCollisionComponent.h"

using namespace std;
using namespace DirectX;

ARenderFrustumCollisionComponent::ARenderFrustumCollisionComponent(
	const string& componentName, 
	const uint32_t& componentID, 
	const DirectX::XMFLOAT3& localPosition, 
	const DirectX::XMFLOAT3& localAngle, 
	const float& fovAngle, 
	const float& nearZ,
	const float& farZ
)
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f))
{
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(localAngle.x, localAngle.y, localAngle.z);
	XMFLOAT4 orientation;
	XMStoreFloat4(&orientation, rotationQuaternion);
	SetBoundingProperties(localPosition, orientation, fovAngle, nearZ, farZ);
}

void ARenderFrustumCollisionComponent::Accept(IComponentVisitor* visitor)
{
	visitor->Visit(this);
}
