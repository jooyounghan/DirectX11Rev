#pragma once
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <concurrent_queue.h>

class DefferedContext
{
public:
	DefferedContext(ID3D11Device** deviceAddress);
	virtual ~DefferedContext() = default;

protected:
	ID3D11Device** m_deviceAddressCached = nullptr;

public:
	void InitDefferedContext();

protected:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_defferedContext;
	Concurrency::concurrent_queue<ID3D11CommandList*> m_commandListsQueue;

public:
	inline ID3D11DeviceContext* GetDefferedContext() { return m_defferedContext.Get(); }
	inline ID3D11DeviceContext** GetDefferedContextAddress() { return m_defferedContext.GetAddressOf(); }

public:
	void RecordToCommandList();
	void TryExecuteCommandList(ID3D11DeviceContext* immediateContext);
};

