#include "DefferedContext.h"

using namespace Microsoft::WRL;

DefferedContext::DefferedContext(ID3D11Device** deviceAddress)
	: m_deviceAddressCached(deviceAddress)
{
}

void DefferedContext::InitDefferedContext()
{
	(*m_deviceAddressCached)->CreateDeferredContext(NULL, m_defferedContext.GetAddressOf());
}

void DefferedContext::RecordToCommandList()
{
	ID3D11CommandList* commandList;
	m_defferedContext->FinishCommandList(TRUE, &commandList);
	m_commandListsQueue.push(commandList);
}

void DefferedContext::TryExecuteCommandList(ID3D11DeviceContext* immediateContext)
{
	ID3D11CommandList* commandList;
	while (m_commandListsQueue.try_pop(commandList))
	{
		immediateContext->ExecuteCommandList(commandList, TRUE);
		commandList->Release();
	}
}
