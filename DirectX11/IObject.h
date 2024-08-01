#pragma once

class PSOObject;

class IObject
{
public:
	virtual void Render(PSOObject* PSOObjectIn) = 0;
	virtual void UpdateObject(const float& DeltaTimeIn) = 0;
};

