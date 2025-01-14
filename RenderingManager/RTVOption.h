#pragma once
#include "ITextureOption.h"

class RTVOption : public ITextureOption
{
public:
	RTVOption() = default;
	~RTVOption() override = default;

protected:
	constexpr static D3D11_BIND_FLAG GetBindFlag()
	{
		return D3D11_BIND_RENDER_TARGET;
	}

protected:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

public:
	ID3D11RenderTargetView* const GetRTV() const { return m_rtv.Get(); }
};
