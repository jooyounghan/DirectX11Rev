#pragma once
#include "ITextureOption.h"

class DSVOption : public ITextureOption
{
public:
	DSVOption() = default;

protected:
	constexpr static D3D11_BIND_FLAG GetBindFlag()
	{
		return D3D11_BIND_DEPTH_STENCIL;
	}

protected:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

public:
	ID3D11DepthStencilView* GetDSV() const { return m_dsv.Get(); }
};