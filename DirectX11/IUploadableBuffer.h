#pragma once

template<typename T>
class IUploadableBuffer
{
public:
	virtual void Upload(const T& CpuDataIn) = 0;
};