#pragma once

class GraphicsPipeline;

class IWorld
{
public:
	IWorld(GraphicsPipeline* GraphicsPipelineInstance);
	virtual ~IWorld();

public:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;

public:
	virtual void RenderWorld() = 0;
};

