#pragma once
#include <atomic>

class AtomicFlag : protected std::atomic_bool
{
public:
	AtomicFlag(const bool& initialFlag);
	~AtomicFlag() = default;

public:
	inline void SetFlag(const bool& flag) { store(flag); }
	inline bool GetFlag() { return load(); }
	inline bool ConsumeFlag() { return exchange(false); }
};

