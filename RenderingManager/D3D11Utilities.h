#pragma once

namespace Utilities
{
	struct SColor
	{
		SColor(const float& r, const float& g, const float& b, const float& a);
		inline const float* GetAddress() const { return &m_R; }
		float m_R;
		float m_G;
		float m_B;
		float m_A;
	};

}

