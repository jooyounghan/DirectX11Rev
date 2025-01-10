#pragma once

class Scene;

class ISceneVisitor
{
public:
	virtual ~ISceneVisitor() = default;

public:
	virtual void Visit(Scene* scene) = 0;
};

