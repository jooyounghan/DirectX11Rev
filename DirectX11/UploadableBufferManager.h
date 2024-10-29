#pragma once
#include "AUploadableBuffer.h"
#include "IUpdatable.h"
#include "HeaderHelper.h"
#include "StaticAssertHelper.h"

#include <memory>
#include <algorithm>
#include <unordered_map>

class UploadableBufferManager : public IUpdatable
{
public:
	UploadableBufferManager();
	~UploadableBufferManager();

protected:
	std::unordered_map<std::shared_ptr<AUploadableBuffer>, bool> UploadableBufferToUploadFlags;

public:
	template<typename T, typename ...Args>
	std::shared_ptr<T> CreateUploadableBuffer(const Args&... args);

protected:
	UpdateReadyHandler OnUploadFlagUpdated;
	void SetUploadFlag(AUploadableBuffer* UploadBufferIn);

public:
	BeginRemoveHandler OnUploadableBufferRemoved;
	void RemoveUploadableBuffer(AManagedByContainer* RemovedBuffer);

public:
	virtual void Update(const float& DeltaTimeIn) override;
};

template<typename T, typename ...Args>
inline std::shared_ptr<T> UploadableBufferManager::CreateUploadableBuffer(const Args & ...args)
{
	static_assert(std::is_base_of<AUploadableBuffer, T>::value, DerivedCondition(AUploadableBuffer));
	std::shared_ptr<T> UploadableBuffer = std::make_shared<T>(args...);
	UploadableBufferToUploadFlags.emplace(UploadableBuffer, false);
	UploadableBuffer->BeginRemoveEvent += OnUploadableBufferRemoved;
	UploadableBuffer->UpdateReadyEvent += OnUploadFlagUpdated;
	return UploadableBuffer;
}
