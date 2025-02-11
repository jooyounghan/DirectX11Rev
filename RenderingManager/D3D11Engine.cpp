#include "D3D11Engine.h"
#include "DirectXUtilities.h"

using namespace D3D11;

D3D11::D3D11Engine::~D3D11Engine()
{
    for (auto& DeferredContexts : m_DeferredContexts)
    {
        delete DeferredContexts.second;
    }
}

D3D11Engine* D3D11Engine::GetInstance()
{
	static D3D11Engine engine;
	return &engine;
}

void D3D11::D3D11Engine::AddDeferredContext(const size_t& DeferredContextID)
{
    if (m_DeferredContexts.find(DeferredContextID) != m_DeferredContexts.end())
    {
        delete m_DeferredContexts[DeferredContextID];
    }
    m_DeferredContexts[DeferredContextID] = new DeferredContext(m_device.GetAddressOf());
}

DeferredContext* D3D11::D3D11Engine::GetDeferredContext(const size_t& DeferredContextID)
{
    if (m_DeferredContexts.find(DeferredContextID) != m_DeferredContexts.end())
    {
        return m_DeferredContexts[DeferredContextID];
    }
    return nullptr;
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

    LoadBackBufferFromSwapchain();

    for (auto& DeferredContexts : m_DeferredContexts)
    {
        DeferredContexts.second->InitDeferredContext();
    }
}

void D3D11Engine::D3D11Engine::ResizeSwapChain(const UINT& widthIn, const UINT& heightIn)
{
    ResetRTVFromBackBuffer();

    m_backBufferTexture.Reset();
    m_backBufferRTV.Reset();

    m_deviceContext->Flush();

    m_swapChain->ResizeBuffers(
        BackBufferCount,
        widthIn, heightIn,
        BackBufferFormat,
        SwapChainFlag
    );

    LoadBackBufferFromSwapchain();
}

void D3D11::D3D11Engine::ClearBackBuffer(const Utilities::SColor& clearColor)
{
    m_deviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor.GetAddress());
}

void D3D11::D3D11Engine::SetRTVAsBackBuffer()
{
    m_deviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
}

void D3D11::D3D11Engine::ResetRTVFromBackBuffer()
{
    ID3D11RenderTargetView* const rtv = nullptr;
    m_deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
}

void D3D11::D3D11Engine::LoadBackBufferFromSwapchain()
{
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBufferTexture.GetAddressOf()));
    AssertIfFailed(m_device->CreateRenderTargetView(m_backBufferTexture.Get(), nullptr, m_backBufferRTV.GetAddressOf()));

}
