#include "AModifiable.h"

void AModifiable::SetIsModified(const bool& isModified)
{
	m_isModified.store(isModified);
}
