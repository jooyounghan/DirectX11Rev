#pragma once

class PSOObject;

class IRenderable
{
public:
	virtual void Render(PSOObject* PSOObjectIn) = 0;
};

