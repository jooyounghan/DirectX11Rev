#pragma once
#include "HeaderHelper.h"

struct SColor
{
	float R;
	float G;
	float B;
	float A;
};

class IDColor
{
public:
	IDColor(const unsigned int& IDIn);

protected:
	unsigned int ID;
	MakeGetter(ID);

public:
	SColor GetColor();
};

