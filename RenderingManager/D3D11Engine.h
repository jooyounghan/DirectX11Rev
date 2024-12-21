#pragma once
#include "D3D11Utilities.h"

#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>

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
		static D3D11Engine* GetInstance();

	protected:
		Microsoft::WRL::ComPtr<ID3D11Device>		m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>		m_swapChain;

	protected:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

	public:
		inline ID3D11Device*			GetDevice() { return m_device.Get(); }
		inline ID3D11DeviceContext*		GetDeviceContext() { return m_deviceContext.Get(); }
		inline ID3D11Device**			GetDeviceAddress() { return m_device.GetAddressOf(); }
		inline ID3D11DeviceContext**	GetDeviceContextAddress() { return m_deviceContext.GetAddressOf(); }
		inline IDXGISwapChain*			GetSwapChain() { return m_swapChain.Get(); }

	public:
		inline ID3D11Texture2D*			GetBackBufferTexture() { return m_backBufferTexture.Get(); }
		inline ID3D11RenderTargetView*	GetBackBufferRTV() { return m_backBufferRTV.Get(); }

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

