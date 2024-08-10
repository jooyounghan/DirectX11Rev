#pragma once

struct SColor
{
	float R;
	float G;
	float B;
	float A;
};

class IDColorProxy
{
public:
	IDColorProxy(const unsigned int& IDIn);
	IDColorProxy(const SColor& ColorIn);

protected:
	unsigned int ID;

public:
	SColor GetColor();
};

