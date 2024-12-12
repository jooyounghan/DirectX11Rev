#pragma once

namespace Utilities
{
	struct SColor
	{
		SColor(const float& r, const float& g, const float& b, const float& a);
		inline const float* GetAddress() const { return &R; }
		float R;
		float G;
		float B;
		float A;
	};

}

