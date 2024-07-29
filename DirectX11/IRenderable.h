#pragma once

class PSOObject;

class IRenderable
{
	virtual void Render(PSOObject* PSOObjectIn) = 0;
};

