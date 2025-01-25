#pragma once
#include "AMeshGPUAsset.h"

#include <DirectXMath.h>

class ScreenQuad : public AMeshGPUAsset
{
private:
	ScreenQuad();
	ScreenQuad(const ScreenQuad& screenQuad) = delete;
	ScreenQuad(ScreenQuad&& screenQuad) = delete;

public:
	static ScreenQuad* GetInstance();

public:
	void InitScreenQuad(ID3D11Device* device);

public:
	virtual std::vector<UINT> GetStrides() const override;
	virtual std::vector<UINT> GetOffsets() const override;
	inline constexpr UINT GetIndexCount() const { return 6; };

private:
	const std::vector<DirectX::XMFLOAT3> m_screenQuadPositions = 
	{ 
		{-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}, {-1.f, -1.f, 1.f}, 
		{-1.f, -1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, -1.f, 1.f}
	};
	const std::vector<DirectX::XMFLOAT2> m_screenQuadUVs =
	{
		{0.f, 0.f}, {1.f, 0.f}, {0.f, 1.f},
		{0.f, 1.f}, {1.f, 0.f}, {1.f, 1.f},
	};
	const std::vector<uint32_t> m_indices =
	{
		 0, 1, 2, 3, 4, 5
	};
};

