#include "IDColorProxy.h"

IDColorProxy::IDColorProxy(const unsigned int& IDIn)
	: ID(IDIn)
{
}

IDColorProxy::IDColorProxy(const SColor& ColorIn)
	: ID(
		static_cast<unsigned int>(ColorIn.R * 255) | 
		static_cast<unsigned int>(ColorIn.G * 255) << 8 | 
		static_cast<unsigned int>(ColorIn.B * 255) << 16 | 
		static_cast<unsigned int>(ColorIn.A * 255) << 24
	)
{
	
}

SColor IDColorProxy::GetColor()
{
	SColor Result;
	Result.R = (ID & 0xFF) / 255.f;
	Result.G = ((ID >> 8) & 0xFF) / 255.f;
	Result.B = ((ID >> 16) & 0xFF) / 255.f;
	Result.A = ((ID >> 24) & 0xFF) / 255.f;
	return Result;
}
