#pragma once
#include "IndexBuffer.h"
#include <vector>

template<typename T>
class Indexable
{
public:
	std::vector<T> Indices;
	IndexBuffer<T> IndicesBuffer;

public:
	inline ID3D11Buffer* GetIndexBuffer() { return IndicesBuffer.GetBuffer(); }
	inline UINT GetIndexCount() { return static_cast<UINT>(Indices.size()); }

};

