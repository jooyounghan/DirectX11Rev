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

protected:
	static const UINT BackBufferCount = 3;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBufferTexture;
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

