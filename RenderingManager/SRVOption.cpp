#include "SRVOption.h"
#include <unordered_map>

using namespace std;

void SRVOption::InitializeByOption(
    ID3D11Device* device,
    ID3D11DeviceContext* deviceContext,
    ID3D11Resource* resource
)
{
    ID3D11Texture2D* texture2D = nullptr;
    HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2D));

    if (SUCCEEDED(hr)) 
    {
        D3D11_TEXTURE2D_DESC texture2DDesc;
        texture2D->GetDesc(&texture2DDesc);
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = GetShaderResourceViewDesc(texture2DDesc);
        device->CreateShaderResourceView(resource, &srvDesc, &m_srv);

        if (texture2DDesc.MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)
        {
        	deviceContext->GenerateMips(m_srv.Get());
        }        
        texture2D->Release();
    }
}

D3D11_SHADER_RESOURCE_VIEW_DESC SRVOption::GetShaderResourceViewDesc(const D3D11_TEXTURE2D_DESC& texture2dDesc)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = GetShaderResourceViewFormatFromTextureFormat(texture2dDesc.Format);

    if (texture2dDesc.SampleDesc.Count == 1) 
    {
        if (texture2dDesc.ArraySize > 1)
        {
            if (texture2dDesc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
            {
                if (texture2dDesc.ArraySize == 6)
                {
                    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                }
                else
                {
                    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                }
            }
            else
            {
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            }
            srvDesc.Texture2DArray.MostDetailedMip = 0;
            srvDesc.Texture2DArray.MipLevels = texture2dDesc.MipLevels;
            srvDesc.Texture2DArray.FirstArraySlice = 0;
            srvDesc.Texture2DArray.ArraySize = texture2dDesc.ArraySize;
        }
        else 
        {
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = texture2dDesc.MipLevels;
        }
    }
    else 
    {
        if (texture2dDesc.ArraySize > 1) 
        {
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            srvDesc.Texture2DMSArray.FirstArraySlice = 0;
            srvDesc.Texture2DMSArray.ArraySize = texture2dDesc.ArraySize;
        }
        else 
        {
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
        }
    }
    return srvDesc;
}

DXGI_FORMAT SRVOption::GetShaderResourceViewFormatFromTextureFormat(const DXGI_FORMAT& textureFormat)
{
    static unordered_map<DXGI_FORMAT, DXGI_FORMAT> textureFormatToSRVFormat{
        { DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT }
    };

    if (textureFormatToSRVFormat.find(textureFormat) != textureFormatToSRVFormat.end())
    {
        return textureFormatToSRVFormat[textureFormat];
    }
    return textureFormat;
}
