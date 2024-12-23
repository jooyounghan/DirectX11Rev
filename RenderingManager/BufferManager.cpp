
#include "BufferManager.h"

using namespace std;

size_t hash<GUID>::operator()(const GUID& Value) const
{
	return size_t();
}

BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
}

GUID BufferManager::RegisterConstantBuffer(
	ID3D11Device* device, 
	const UINT& elementSize, 
	const UINT& arrayCount,
	void* cpuDataIn,
	const D3D11_BIND_FLAG& bindFlag,
	const D3D11_USAGE& usage
)
{
	GUID guid = IssueGuid();
	m_guidToRegisteredConstantBuffers.emplace(
		guid, 
		RegisterWithCreateHelper<ConstantBuffer>(
			device, elementSize, arrayCount, cpuDataIn, bindFlag, usage
		));
	return guid;
}

GUID BufferManager::RegisterDynamicBuffer(
	ID3D11Device* device, 
	const UINT& elementSize, 
	const UINT& arrayCount
)
{
	GUID guid = IssueGuid();
	m_guidToRegisteredUploadableBuffers.emplace(
		guid, 
		RegisterWithCreateHelper<DynamicBuffer>(
			device, elementSize, arrayCount
		));
	return guid;
}

GUID BufferManager::RegisterStructureBuffer(
	ID3D11Device* device, 
	const UINT& elementSize, 
	const UINT& arrayCount
)
{
	GUID guid = IssueGuid();
	m_guidToRegisteredUploadableBuffers.emplace(
		guid,
		RegisterWithCreateHelper<StructureBuffer>(
			device, elementSize, arrayCount
		));
	return guid;
}

GUID BufferManager::IssueGuid()
{
	GUID guid;
	AssertIfFailed(CoCreateGuid(&guid));
	return guid;
}

template<typename IsInitializableBuffer, typename ...Args>
inline unique_ptr<IsInitializableBuffer> BufferManager::RegisterWithCreateHelper(ID3D11Device* device, Args ...args)
{
	unique_ptr<IsInitializableBuffer> buffer = make_unique<IsInitializableBuffer>(args...);
	buffer->Initialize(device);
	return buffer;
}


ConstantBuffer* BufferManager::GetRegisteredConstantBuffer(const GUID& guid)
{
	if (m_guidToRegisteredConstantBuffers.find(guid) != m_guidToRegisteredConstantBuffers.end())
	{
		return m_guidToRegisteredConstantBuffers[guid].get();
	}
	return nullptr;
}

AUploadableBuffer* BufferManager::GetRegisteredUploadableBuffer(const GUID& guid)
{
	if (m_guidToRegisteredUploadableBuffers.find(guid) != m_guidToRegisteredUploadableBuffers.end())
	{
		return m_guidToRegisteredUploadableBuffers[guid].get();
	}
	return nullptr;
}

void BufferManager::RequestUpload(const GUID& guid, ID3D11DeviceContext* deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn)
{
	UploadRequestArgs uploadRequestArgs;
	AutoZeroMemory(uploadRequestArgs);

	AUploadableBuffer* uploadableBuffer = GetRegisteredUploadableBuffer(guid);

	if (uploadableBuffer != nullptr)
	{
		uploadRequestArgs.m_uploadableBuffer = uploadableBuffer;
		uploadRequestArgs.m_deviceContextCached = deviceContext;
		uploadRequestArgs.m_elementSize = elementSize;
		uploadRequestArgs.m_arrayCount = arrayCount;
		uploadRequestArgs.m_cpuDataIn = cpuDataIn;

		m_uploadRequestedArgs.emplace_back(move(uploadRequestArgs));
	}
}

void BufferManager::ExecuteUpload()
{
	for (const UploadRequestArgs& uploadRequestedArg : m_uploadRequestedArgs)
	{
		uploadRequestedArg.m_uploadableBuffer->Upload(
			uploadRequestedArg.m_deviceContextCached,
			uploadRequestedArg.m_elementSize,
			uploadRequestedArg.m_arrayCount,
			uploadRequestedArg.m_cpuDataIn
		);
	}
	m_uploadRequestedArgs.clear();
}

