
#include "ABuffer.h"

ABuffer::ABuffer(const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn)
	: m_elementSize(elementSize), m_arrayCount(arrayCount), m_cpuDataIn(cpuDataIn)
{
}