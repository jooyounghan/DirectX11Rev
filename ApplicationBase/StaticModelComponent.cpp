#include "StaticModelComponent.h"

using namespace std;

StaticModelComponent::StaticModelComponent(const StaticMeshAsset* const staticMeshAsset)
	: m_staticMeshAsset(staticMeshAsset)
{
	m_selectedMaterials = m_staticMeshAsset->GetDefaultMaterials();
}