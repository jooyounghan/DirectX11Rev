#pragma once

#include <atomic>

class AModifiable
{
protected:
	std::atomic_bool m_isModified = false;

public:
	inline const bool ComsumeIsModified() { return m_isModified.exchange(false, std::memory_order_acquire); }
	virtual void SetIsModified(const bool& isModified) = 0;
};

