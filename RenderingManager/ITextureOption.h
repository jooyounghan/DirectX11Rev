#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <type_traits>

template <class T>
concept IsTextureOption = requires (T option, ID3D11Resource * resource, ID3D11Device * device)
{
	T::GetBindFlag();
	option.InitializeByOption(resource, device);
};

class ITextureOption
{
protected:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext) = 0;
};

class PureTextureOption : public ITextureOption
{
protected:
	constexpr static D3D11_BIND_FLAG GetBindFlag()
	{
		return (D3D11_BIND_FLAG)NULL;
	}

protected:
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext) override {};

};
