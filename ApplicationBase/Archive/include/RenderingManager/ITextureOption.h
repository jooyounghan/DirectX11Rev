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
	virtual void InitializeByOption(ID3D11Resource* resource, ID3D11Device* device) = 0;
};


