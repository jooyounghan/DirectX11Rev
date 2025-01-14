
#include "SRVOption.h"

void SRVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	device->CreateShaderResourceView(resource, NULL, &m_srv);

    ID3D11Texture2D* texture2D = nullptr;
    HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2D));

    if (SUCCEEDED(hr)) 
    {
        D3D11_TEXTURE2D_DESC texture2DDesc;
        texture2D->GetDesc(&texture2DDesc);
        if (texture2DDesc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)
        {
        	deviceContext->GenerateMips(m_srv.Get());
        }        
        texture2D->Release();
    }
}
