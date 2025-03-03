#include "AtomicFlag.h"

AtomicFlag::AtomicFlag(const bool& initialFlag) : std::atomic_bool(initialFlag) {}
