#include "ALightComponent.h"

using namespace std;
using namespace DirectX;

ALightComponent::ALightComponent(
	const std::string& componentName, 
	const uint32_t& componentID, 
	const DirectX::XMFLOAT3& localPosition, 
	const DirectX::XMFLOAT3& localAngle
)
	: AComponent(componentName, componentID, localPosition, localAngle, XMFLOAT3(1.f, 1.f, 1.f))
{
}
