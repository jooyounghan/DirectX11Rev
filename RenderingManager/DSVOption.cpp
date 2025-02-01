#include "DSVOption.h"
#include <unordered_map>

using namespace std;

void DSVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    ID3D11Texture2D* texture2D = nullptr;
    HRESULT hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2D));

    if (SUCCEEDED(hr))
    {
        D3D11_TEXTURE2D_DESC texture2DDesc;
        texture2D->GetDesc(&texture2DDesc);
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = GetDepthStencilViewDesc(texture2DDesc);
        device->CreateDepthStencilView(resource, &dsvDesc, &m_dsv);

        texture2D->Release();
    }
}

D3D11_DEPTH_STENCIL_VIEW_DESC DSVOption::GetDepthStencilViewDesc(const D3D11_TEXTURE2D_DESC& texture2dDesc)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));

    dsvDesc.Format = GetDepthStencilViewFormatFromTextureFormat(texture2dDesc.Format);

    if (texture2dDesc.SampleDesc.Count == 1)
    {
        if (texture2dDesc.ArraySize > 1)
        {
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
            dsvDesc.Texture2DArray.MipSlice = 0;
            dsvDesc.Texture2DArray.FirstArraySlice = 0;
            dsvDesc.Texture2DArray.ArraySize = texture2dDesc.ArraySize;
        }
        else
        {
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Texture2D.MipSlice = 0;
        }
    }
    else
    {
        if (texture2dDesc.ArraySize > 1)
        {
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
            dsvDesc.Texture2DMSArray.FirstArraySlice = 0;
            dsvDesc.Texture2DMSArray.ArraySize = texture2dDesc.ArraySize;
        }
        else
        {
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        }
    }
    return dsvDesc;
}

DXGI_FORMAT DSVOption::GetDepthStencilViewFormatFromTextureFormat(const DXGI_FORMAT& textureFormat)
{
    static unordered_map<DXGI_FORMAT, DXGI_FORMAT> textureFormatToDSVFormat{
        { DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT }
    };

    if (textureFormatToDSVFormat.find(textureFormat) != textureFormatToDSVFormat.end())
    {
        return textureFormatToDSVFormat[textureFormat];
    }
    return textureFormat;

}
