#pragma once

#include "HeaderHelper.h"

#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include<memory>


class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();

protected:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	MakeComPtrGetter(Device);
	MakeComPtrGetter(DeviceContext);
	MakeComPtrGetter(SwapChain);

public:
	static const UINT BackBufferCount = 3;
	static const DXGI_FORMAT BackBufferFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	static const UINT SwapChainFlag = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBufferTexture;
	MakeComPtrGetter(BackBufferTexture);

protected:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	MakeComPtrGetter(BackBufferRTV);

protected:
	float AspectRatio = 0.f;
	MakeSetterGetter(AspectRatio);

public:
	void LoadPipeline(const UINT& WidthIn, const UINT& HeightIn, HWND WndHandleIn);
	
private:
	void CreateDeviceAndSwapChain(const UINT& WidthIn, const UINT& HeightIn, HWND WndHandleIn);
	void SetBackBuffer();

public:
	void ResizeSwapChain(const UINT& WidthIn, const UINT& HeightIn);
};

