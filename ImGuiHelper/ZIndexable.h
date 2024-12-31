#pragma once
class ZIndexable
{
protected:
	int m_zIndex = 0;

public:
	inline const int& GetZIndex() { return m_zIndex; }
};

