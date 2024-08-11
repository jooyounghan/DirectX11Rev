#pragma once
#include "VertexBuffer.h"
#include <vector>

template <typename T>
class Vertexable
{
public:
	std::vector<T>		Vertices;

public:
	VertexBuffer<T>		VerticesBuffer;

public:
	inline ID3D11Buffer* GetVertexBuffer() { return VerticesBuffer.GetBuffer(); }
	inline UINT GetVertexTypeSize() { return sizeof(T); };
	inline UINT GetVertexCount() { return static_cast<UINT>(Vertices.size()); }
};

