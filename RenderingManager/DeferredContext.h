#pragma once
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <concurrent_queue.h>

class DeferredContext
{
public:
	DeferredContext(ID3D11Device* const* deviceAddress);
	virtual ~DeferredContext() = default;

protected:
	ID3D11Device* const* m_deviceAddressCached = nullptr;

public:
	void InitDeferredContext();

protected:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deferredContext;
	Concurrency::concurrent_queue<ID3D11CommandList*> m_commandListsQueue;

public:
	inline ID3D11DeviceContext* GetDeferredContext() { return m_deferredContext.Get(); }
	inline ID3D11DeviceContext* const* GetDeferredContextAddress() { return m_deferredContext.GetAddressOf(); }

public:
	void RecordToCommandList();
	void TryExecuteCommandList(ID3D11DeviceContext* immediateContext);
};

