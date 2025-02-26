#include "DeferredContext.h"

using namespace Microsoft::WRL;

DeferredContext::DeferredContext(ID3D11Device* const* deviceAddress)
	: m_deviceAddressCached(deviceAddress)
{
}

void DeferredContext::InitDeferredContext()
{
	(*m_deviceAddressCached)->CreateDeferredContext(NULL, m_deferredContext.GetAddressOf());
}

void DeferredContext::RecordToCommandList()
{
	ID3D11CommandList* commandList;
	m_deferredContext->FinishCommandList(TRUE, &commandList);
	m_commandListsQueue.push(commandList);
}

void DeferredContext::TryExecuteCommandList(ID3D11DeviceContext* immediateContext)
{
	ID3D11CommandList* commandList;
	while (m_commandListsQueue.try_pop(commandList))
	{
		if (commandList)
		{
			immediateContext->ExecuteCommandList(commandList, TRUE);
			commandList->Release();
		}
	}
}
