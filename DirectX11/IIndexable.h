#pragma once
#include "IndexBuffer.h"

#include <vector>

template<typename T>
class IIndexable
{
public:
	std::vector<T> Indices;

protected:
	IndexBuffer<T> IndicesBuffer;

public:
	inline ID3D11Buffer* GetIndexBuffer() { return IndicesBuffer.GetBuffer(); }
	inline UINT GetIndexCount() { return static_cast<UINT>(Indices.size()); }

public:
	virtual DXGI_FORMAT GetIndexFormat() = 0;
	virtual void Initialize(ID3D11Device* DeviceIn) = 0;
};

