#pragma once
class ZIndexable
{
public:
	virtual ~ZIndexable() = default;

protected:
	int m_zIndex = 0;

public:
	inline void SetZIndex(const int& zIndex) { m_zIndex = zIndex; }
	inline const int& GetZIndex() { return m_zIndex; }
};

