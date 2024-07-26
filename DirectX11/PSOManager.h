#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <memory>

enum EPSOType : size_t
{
	R8G8B8A8_Static_Solid,
	R8G8B8A8_Static_Wireframe,
	R8G8B8A8_Skeletal_Solid,
	R8G8B8A8_Skeletal_Wireframe,
	PSOTypeCount
};

class GraphicsPipeline;
class PSOObject;

class PSOManager
{
public:
	PSOManager(GraphicsPipeline* GraphicsPipelineIn);
	~PSOManager();

private:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;

protected:
	std::unique_ptr<PSOObject> PSOObjects[PSOTypeCount];

public:
	inline PSOObject* GetPSOObject(EPSOType PsoTypeIn) { return PSOObjects[PsoTypeIn].get(); }

private:
	void CreateStatic();
	void CreateSkeletal();
};

