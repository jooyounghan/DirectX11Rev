#include "D3D11Engine.h"
#include "RenderingManager/DirectXUtilities.h"

using namespace D3D11;

D3D11Engine* D3D11Engine::g_engine = nullptr;

D3D11Engine** D3D11Engine::GetInstance()
{
	static D3D11Engine engine;
    g_engine = &engine;
	return &g_engine;
}

void D3D11Engine::D3D11Engine::InitEngine(const UINT& widthIn, const UINT& heightIn, HWND wndHandle)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    AutoZeroMemory(swapChainDesc);

    swapChainDesc.BufferDesc.Width = widthIn;
    swapChainDesc.BufferDesc.Height = heightIn;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshNumerator;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = RefreshDenominator;
    swapChainDesc.BufferDesc.Format = BackBufferFormat;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = BackBufferCount;
    swapChainDesc.OutputWindow = wndHandle;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = SwapEffect;
    swapChainDesc.Flags = SwapChainFlag;

    D3D_FEATURE_LEVEL selectedFeatureLevel;
    AssertIfFailed(D3D11CreateDeviceAndSwapChain(
        NULL,
        DriverType,
        NULL,
        CreateDeviceFlags,
        FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &selectedFeatureLevel,
        m_deviceContext.GetAddressOf()
    ));

    assert(m_swapChain != nullptr);
    assert(m_device != nullptr);
    assert(m_deviceContext != nullptr);

    m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBufferTexture.GetAddressOf()));
    AssertIfFailed(m_device->CreateRenderTargetView(m_backBufferTexture.Get(), nullptr, m_backBufferRTV.GetAddressOf()));

    LoadBackBufferFromSwapchain();
}

void D3D11Engine::D3D11Engine::ResizeSwapChain(const UINT& widthIn, const UINT& heightIn)
{
    m_backBufferTexture->Release();
    m_backBufferRTV->Release();

    m_swapChain->ResizeBuffers(
        BackBufferCount,
        widthIn, heightIn,
        BackBufferFormat,
        SwapChainFlag
    );

    LoadBackBufferFromSwapchain();
}

void D3D11::D3D11Engine::LoadBackBufferFromSwapchain()
{
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBufferTexture.GetAddressOf()));
    AssertIfFailed(m_device->CreateRenderTargetView(m_backBufferTexture.Get(), nullptr, m_backBufferRTV.GetAddressOf()));

}
