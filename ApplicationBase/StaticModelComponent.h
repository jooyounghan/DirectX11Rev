#pragma once
#include "AModelComponent.h"
#include "StaticMeshAsset.h"

class StaticModelComponent : public AModelComponent
{
public:
	StaticModelComponent(const StaticMeshAsset* const staticMeshAsset);

protected:
	const StaticMeshAsset* const m_staticMeshAsset;

};

