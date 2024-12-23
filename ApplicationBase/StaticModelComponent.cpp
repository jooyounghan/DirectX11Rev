#include "StaticModelComponent.h"

using namespace std;
using namespace DirectX;

StaticModelComponent::StaticModelComponent(
	const uint64_t& componentID, XMFLOAT3 position, 
	const StaticMeshAsset* const staticMeshAsset
)
	: AModelComponent(componentID, position), m_staticMeshAsset(staticMeshAsset)
{
	m_selectedMaterials = m_staticMeshAsset->GetDefaultMaterials();
}