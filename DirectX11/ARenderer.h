#pragma once
#include "HeaderHelper.h"
#include <list>

class PSOObject;

class MeshObject;
class ABoundingComponent;
class Map;
class Camera;

class ARenderer
{
public:
	ARenderer(PSOObject* PSOObjectIn) : PSOObjectCached(PSOObjectIn) {};

protected:
	PSOObject* PSOObjectCached = nullptr;

public:
	virtual void PresetRendering(Camera* CameraIn, Map* MapIn) = 0;
	virtual void ResetRendering() = 0;

public:
	virtual void Render(MeshObject* MeshObjectIn) = 0;
	virtual void Render(ABoundingComponent* MeshObjectIn) = 0;
};
