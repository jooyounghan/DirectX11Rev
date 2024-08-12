#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>

enum EPSOType : size_t
{
	// Position Only
	R8G8B8A8_BoundingComponent_Solid,
	R8G8B8A8_BoundingComponent_Wireframe,
	R32_Picking_ID,

	// Static
	R8G8B8A8_Static_Solid,
	R8G8B8A8_Static_Wireframe,

	// Skeletal
	R8G8B8A8_Skeletal_Solid,
	R8G8B8A8_Skeletal_Wireframe,
	PSOTypeCount
};

class GraphicsPipeline;
class PSOObject;
class ARenderer;

class PSOManager
{
public:
	PSOManager(GraphicsPipeline* GraphicsPipelineIn);
	~PSOManager();

private:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;

protected:
	std::unique_ptr<PSOObject> PSOObjects[PSOTypeCount];
	std::unique_ptr<ARenderer> Renderers[PSOTypeCount];

public:
	inline PSOObject* GetPSOObject(EPSOType PsoTypeIn) { return PSOObjects[PsoTypeIn].get(); }
	inline ARenderer* GetRenderers(EPSOType PsoTypeIn) { return Renderers[PsoTypeIn].get(); }

private:
	void CreatePositionOnlyPSO();
	void CreateStatic();
	void CreateSkeletal();
	void CreateRenderers();
};

