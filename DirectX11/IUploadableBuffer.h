#pragma once

struct ID3D11DeviceContext;

template<typename T>
class IUploadableBuffer
{
public:
	virtual void Upload(ID3D11DeviceContext* DeviceContextIn, const T& CpuDataIn) = 0;
};