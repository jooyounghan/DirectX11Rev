#pragma once

class IUpdatable
{
public:
	virtual void Update(const float& DeltaTimeIn) = 0;
};