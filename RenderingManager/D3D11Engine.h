#pragma once
#include "D3D11Utilities.h"
#include "unordered_map"
#include "DeferredContext.h"

namespace D3D11
{
	constexpr UINT				BackBufferCount = 3;
	constexpr UINT				RefreshNumerator = 240;
	constexpr UINT				RefreshDenominator = 1;
	constexpr DXGI_FORMAT		BackBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	constexpr UINT				SwapChainFlag = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	constexpr DXGI_SWAP_EFFECT	SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

#if defined(DEBUG) || defined(_DEBUG)
	constexpr UINT				CreateDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
	constexpr UINT				CreateDeviceFlags = NULL;
#endif
	constexpr D3D_DRIVER_TYPE	DriverType = D3D_DRIVER_TYPE_HARDWARE;
	constexpr D3D_FEATURE_LEVEL FeatureLevels[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	
	class D3D11Engine
	{
	private:
		D3D11Engine() = default;
		D3D11Engine(const D3D11Engine& engine) = delete;
		D3D11Engine(D3D11Engine&& engine) = delete;

	public:
		~D3D11Engine();

	public:
		static D3D11Engine* GetInstance();

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device>		m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>		m_swapChain;

	protected:
		std::unordered_map<size_t, DeferredContext*> m_DeferredContexts;

	public:
		void AddDeferredContext(const size_t& DeferredContextID);
		DeferredContext* GetDeferredContext(const size_t& DeferredContextID);

	protected:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

	public:
		inline ID3D11Device*			GetDevice() const { return m_device.Get(); }
		inline ID3D11DeviceContext*		GetDeviceContext() const { return m_deviceContext.Get(); }
		inline ID3D11Device* const*			GetDeviceAddress() const { return m_device.GetAddressOf(); }
		inline ID3D11DeviceContext* const*	GetDeviceContextAddress() const { return m_deviceContext.GetAddressOf(); }
		inline IDXGISwapChain*			GetSwapChain() const { return m_swapChain.Get(); }

	public:
		inline ID3D11Texture2D*			GetBackBufferTexture() const { return m_backBufferTexture.Get(); }
		inline ID3D11RenderTargetView*	GetBackBufferRTV() const { return m_backBufferRTV.Get(); }
		inline ID3D11Texture2D* const*		GetBackBufferTextureAddress() const { return m_backBufferTexture.GetAddressOf(); }
		inline ID3D11RenderTargetView* const* GetBackBufferRTVAddress() const { return m_backBufferRTV.GetAddressOf(); }

	public:
		void InitEngine(const UINT& widthIn, const UINT& heightIn, HWND wndHandle);
		void ResizeSwapChain(const UINT& widthIn, const UINT& heightIn);
		void ClearBackBuffer(const Utilities::SColor& clearColor);
		void SetRTVAsBackBuffer();
		void ResetRTVFromBackBuffer();

	private:
		void LoadBackBufferFromSwapchain();
	};

}

