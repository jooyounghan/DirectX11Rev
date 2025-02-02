#pragma once
#include "AMeshData.h"

class ScreenQuad : public AMeshData
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
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffers() const override;
	virtual std::vector<ConstantBuffer*> GetVertexConstantBuffersForDepthTest() const override;

public:
	virtual std::vector<UINT> GetStrides() const override;
	virtual std::vector<UINT> GetOffsets() const override;
	virtual std::vector<UINT> GetStridesForDepthTest() const override;
	virtual std::vector<UINT> GetOffsetsForDepthTest() const override;
	inline constexpr UINT GetIndexCount() const { return 6; };
};

