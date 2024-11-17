#include "GraphicsPipeline.h"
#include "GlobalVariable.h"
#include "DefineUtility.h"

#include <filesystem>
#include <shlobj.h>

using namespace std;
using namespace Microsoft::WRL;

GraphicsPipeline::GraphicsPipeline()
{
    App::GGraphicPipeline = this;
}

GraphicsPipeline::~GraphicsPipeline()
{
}


void GraphicsPipeline::LoadPipeline(const UINT& WidthIn, const UINT& HeightIn, HWND WndHandleIn)
{
    SetAspectRatio(static_cast<float>(WidthIn) / HeightIn);
    CreateDeviceAndSwapChain(WidthIn, HeightIn, WndHandleIn);
    SetBackBuffer();
}

void GraphicsPipeline::CreateDeviceAndSwapChain(const UINT& WidthIn, const UINT& HeightIn, HWND WndHandleIn)
{
    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
    const D3D_FEATURE_LEVEL featureLevels[3] = { D3D_FEATURE_LEVEL_11_1 , D3D_FEATURE_LEVEL_11_0 };    
    D3D_FEATURE_LEVEL selectedFeatureLevel;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    AutoZeroMemory(swapChainDesc);
    swapChainDesc.BufferDesc.Width = WidthIn;
    swapChainDesc.BufferDesc.Height = HeightIn;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 240;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = BackBufferCount;
    swapChainDesc.OutputWindow = WndHandleIn;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    AssertIfFailed(D3D11CreateDeviceAndSwapChain(
        NULL,
        driverType,
        NULL,
        createDeviceFlags,
        featureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        SwapChain.GetAddressOf(),
        Device.GetAddressOf(),
        &selectedFeatureLevel,
        DeviceContext.GetAddressOf()
    ));

    assert(SwapChain != nullptr);
    assert(Device != nullptr);
    assert(DeviceContext != nullptr);
}

void GraphicsPipeline::SetBackBuffer()
{
    SwapChain->GetBuffer(0, IID_PPV_ARGS(BackBufferTexture.GetAddressOf()));
    AssertIfFailed(Device->CreateRenderTargetView(BackBufferTexture.Get(), nullptr, BackBufferRTV.GetAddressOf()));
}

void GraphicsPipeline::ResizeSwapChain(const UINT& WidthIn, const UINT& HeightIn)
{
    BackBufferTexture->Release();
    BackBufferRTV->Release();

    SwapChain->ResizeBuffers(
        GraphicsPipeline::BackBufferCount,
        WidthIn, HeightIn,
        GraphicsPipeline::BackBufferFormat,
        GraphicsPipeline::SwapChainFlag
    );
    
    SetBackBuffer();
}
