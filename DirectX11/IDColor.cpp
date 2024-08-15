#include "IDColor.h"

IDColor::IDColor(const unsigned int& IDIn)
	: ID(IDIn)
{
}

SColor IDColor::GetColor()
{
	SColor Result;
	Result.R = (ID & 0xFF) / 255.f;
	Result.G = ((ID >> 8) & 0xFF) / 255.f;
	Result.B = ((ID >> 16) & 0xFF) / 255.f;
	Result.A = ((ID >> 24) & 0xFF) / 255.f;
	return Result;
}
