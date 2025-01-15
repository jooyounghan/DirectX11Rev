#pragma once
#include "ITextureOption.h"

class SRVOption : public ITextureOption
{
public:
	SRVOption() = default;

protected:
	constexpr static D3D11_BIND_FLAG GetBindFlag()
	{
		return D3D11_BIND_SHADER_RESOURCE;
	}

protected:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext) override;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

public:
	inline ID3D11ShaderResourceView* const GetSRV() const { return m_srv.Get(); }
};

