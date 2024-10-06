#pragma once
#include "directxmath/DirectXMath.h"
#include <vector>

class ModelHelper
{
public:
	template<typename IndexType>
	static void CreateSphere(
		const IndexType& SphereLevel,
		std::vector<DirectX::XMFLOAT3>* VerticesIn,
		std::vector<DirectX::XMFLOAT2>* UVCoordsIn,
		std::vector<DirectX::XMFLOAT3>* NormalsIn,
		std::vector<IndexType>* IndicesIn,
		const bool& IsReverse
	);

	template<typename IndexType>
	static void CreateCube(
		std::vector<DirectX::XMFLOAT3>* VerticesIn,
		std::vector<DirectX::XMFLOAT2>* UVCoordsIn,
		std::vector<DirectX::XMFLOAT3>* NormalsIn,
		std::vector<IndexType>* IndicesIn,
		const bool& IsReverse
	);
};

template<typename IndexType>
inline void ModelHelper::CreateSphere(
	const IndexType& SphereLevel,
	std::vector<DirectX::XMFLOAT3>* VerticesIn,
	std::vector<DirectX::XMFLOAT2>* UVCoordsIn,
	std::vector<DirectX::XMFLOAT3>* NormalsIn,
	std::vector<IndexType>* IndicesIn,
	const bool& IsReverse
)
{
	for (IndexType latitudeIdx = 0; latitudeIdx < SphereLevel; ++latitudeIdx)
	{
		const float& fLatitudeLow = DirectX::XM_PIDIV2 / SphereLevel * latitudeIdx;
		const float& fLatitudeHigh = DirectX::XM_PIDIV2 / SphereLevel * (latitudeIdx + 1);
		const float& fLatitudeLowTextureCord = (static_cast<float>(latitudeIdx) / SphereLevel) / 2.f;
		const float& fLatitudeHighTextureCord = (static_cast<float>(latitudeIdx + 1) / SphereLevel) / 2.f;

		const IndexType usLatitudeOffset = (IndexType)VerticesIn->size();

		for (IndexType longitudeIdx = 0; longitudeIdx <= SphereLevel * 2; ++longitudeIdx)
		{
			const float& fLongitudeLow = DirectX::XM_2PI / (SphereLevel * 2.f) * longitudeIdx;
			const float& fLongitudeTextureCord = longitudeIdx / (SphereLevel * 2.f);

			if (VerticesIn != nullptr)
			{
				VerticesIn->emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));
				VerticesIn->emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));
				VerticesIn->emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));
				VerticesIn->emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
			}

			if (UVCoordsIn != nullptr)
			{
				UVCoordsIn->emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord);
				UVCoordsIn->emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord);
				UVCoordsIn->emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord);
				UVCoordsIn->emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord);
			}

			if (NormalsIn != nullptr)
			{
				NormalsIn->emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));
				NormalsIn->emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));
				NormalsIn->emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));
				NormalsIn->emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
			}
		}

		if (IndicesIn != nullptr)
		{
			for (IndexType longitudeIdx = 0; longitudeIdx < SphereLevel * 2; ++longitudeIdx)
			{
				const IndexType& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
				IndicesIn->push_back(usLongitudeOffset + 0);
				IndicesIn->push_back(usLongitudeOffset + 1);
				IndicesIn->push_back(usLongitudeOffset + 4);
				IndicesIn->push_back(usLongitudeOffset + 4);
				IndicesIn->push_back(usLongitudeOffset + 1);
				IndicesIn->push_back(usLongitudeOffset + 5);

				IndicesIn->push_back(usLongitudeOffset + 3);
				IndicesIn->push_back(usLongitudeOffset + 2);
				IndicesIn->push_back(usLongitudeOffset + 7);
				IndicesIn->push_back(usLongitudeOffset + 7);
				IndicesIn->push_back(usLongitudeOffset + 2);
				IndicesIn->push_back(usLongitudeOffset + 6);
			}
		}
	}

	if (IsReverse)
	{
		std::reverse(IndicesIn->begin(), IndicesIn->end());
	}
}

template<typename IndexType>
void ModelHelper::CreateCube(
	std::vector<DirectX::XMFLOAT3>* VerticesIn,
	std::vector<DirectX::XMFLOAT2>* UVCoordsIn,
	std::vector<DirectX::XMFLOAT3>* NormalsIn,
	std::vector<IndexType>* IndicesIn,
	const bool& IsReverse
)
{
	const DirectX::XMFLOAT3 CubeVertices[] = 
	{
		// Front
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },

		// Back
		{ -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },

		// Left
		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f,  1.0f },

		// Right
		{  1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f,  1.0f },

		// Top
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },

		// Bottom
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f }
	};

	const DirectX::XMFLOAT2 CubeUVs[] = 
	{
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },  // Front
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },  // Back
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },  // Left
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },  // Right
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },  // Top
		{ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }   // Bottom
	};

	const DirectX::XMFLOAT3 CubeNormals[] = 
	{
		{ 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f, -1.0f },  // Front
		{ 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f,  0.0f,  1.0f },  // Back
		{ -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f }, { -1.0f,  0.0f,  0.0f },  // Left
		{ 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,  0.0f,  0.0f },  // Right
		{ 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,  1.0f,  0.0f },  // Top
		{ 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }   // Bottom
	};

	const IndexType CubeIndices[] = 
	{
		// Front
		0, 3, 2, 2, 1, 0,
		// Back
		4, 5, 6, 4, 6, 7,
		// Left
		8, 11, 10, 10, 9, 8,
		// Right
		12, 13, 14, 12, 14, 15,
		// Top
		16, 19, 18, 18, 17, 16,
		// Bottom
		20, 21, 22, 20, 22, 23
	};

	if (VerticesIn != nullptr)
	{
		VerticesIn->assign(CubeVertices, CubeVertices + 24);
	}

	if (UVCoordsIn != nullptr)
	{
		UVCoordsIn->assign(CubeUVs, CubeUVs + 24);
	}

	if (NormalsIn != nullptr)
	{
		NormalsIn->assign(CubeNormals, CubeNormals + 24);
	}

	if (IndicesIn != nullptr)
	{
		IndicesIn->assign(CubeIndices, CubeIndices + 36);

		if (IsReverse)
		{
			std::reverse(IndicesIn->begin(), IndicesIn->end());
		}
	}
}