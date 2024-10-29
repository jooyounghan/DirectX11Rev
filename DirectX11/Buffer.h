#pragma once
#include <windows.h>
#include <wrl/client.h>
#include <d3d11.h>

class BaseBuffer
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;

public:
	ID3D11Buffer* GetBuffer() const { return Buffer.Get(); }
};