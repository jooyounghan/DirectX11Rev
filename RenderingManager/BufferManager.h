#pragma once
#include "ConstantBuffer.h"
#include "DynamicBuffer.h"
#include "StructureBuffer.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <objbase.h>
#include <guiddef.h>

namespace std
{
	template<> struct hash<GUID>
	{
		size_t operator()(const GUID& Value) const;
	};
}

template <class T>
concept IsInitializableBuffer = requires (T buffer, ID3D11Device * device)
{
	{ buffer.Initialize(device) } -> std::same_as<void>;
};

struct UploadRequestArgs
{
	AUploadableBuffer* uploadableBuffer;
	ID3D11DeviceContext* deviceContext;
	UINT elementSize; 
	UINT arrayCount;
	void* cpuDataIn;
};

class BufferManager
{
public:
	BufferManager();
	~BufferManager();

public:
	GUID RegisterConstantBuffer(
		ID3D11Device* device,
		const UINT& elementSize, 
		const UINT& arrayCount, void* cpuDataIn, 
		const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE
	);

	GUID RegisterDynamicBuffer(
		ID3D11Device* device,
		const UINT& elementSize,
		const UINT& arrayCount
	);

	GUID RegisterStructureBuffer(
		ID3D11Device* device,
		const UINT& elementSize,
		const UINT& arrayCount
	);

private:
	GUID IssueGuid();
	template<typename IsInitializableBuffer, typename ...Args> std::unique_ptr<IsInitializableBuffer> RegisterWithCreateHelper(
		ID3D11Device* device, Args... args
	);


protected:
	std::unordered_map<GUID, std::unique_ptr<ConstantBuffer>> m_guidToRegisteredConstantBuffers;
	std::unordered_map<GUID, std::unique_ptr<AUploadableBuffer>> m_guidToRegisteredUploadableBuffers;

public:
	ConstantBuffer* GetRegisteredConstantBuffer(const GUID& guid);
	AUploadableBuffer* GetRegisteredUploadableBuffer(const GUID& guid);

public:
	void RequestUpload(const GUID& guid, ID3D11DeviceContext* deviceContext, const UINT& elementSize, const UINT& arrayCount, void* cpuDataIn);

protected:
	std::vector<UploadRequestArgs> m_uploadRequestedArgs;

public:
	void ExecuteUpload();
};

