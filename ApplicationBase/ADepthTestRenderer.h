#pragma once
#include "IComponentVisitor.h"
#include <vector>
struct ID3D11DeviceContext;
class ComponentPSOManager;
class AMeshPartsData;

class ADepthTestRenderer : public IComponentVisitor
{
public:
	ADepthTestRenderer(
		ID3D11DeviceContext* deviceContext,
		ComponentPSOManager* componentPsoManager
	);
	virtual ~ADepthTestRenderer() = default;

protected:
	ID3D11DeviceContext* m_deviceContext = nullptr;
	ComponentPSOManager* m_componentPsoManagerCached = nullptr;


protected:
	void DepthTestMeshParts(const AMeshPartsData* const meshPartsData);
};

