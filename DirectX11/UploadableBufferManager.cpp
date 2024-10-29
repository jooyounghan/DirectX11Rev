#include "UploadableBufferManager.h"

using namespace std;

UploadableBufferManager::UploadableBufferManager()
{
	OnUploadFlagUpdated = bind(&UploadableBufferManager::SetUploadFlag, this, placeholders::_1);
	OnUploadableBufferRemoved = bind(&UploadableBufferManager::RemoveUploadableBuffer, this, placeholders::_1);
}

UploadableBufferManager::~UploadableBufferManager()
{
}

void UploadableBufferManager::SetUploadFlag(AUploadableBuffer* UploadBufferIn)
{
	auto it = std::find_if(
		UploadableBufferToUploadFlags.begin(),
		UploadableBufferToUploadFlags.end(),
		[UploadBufferIn](const auto& pair) {
			return pair.first.get() == UploadBufferIn;
		}
	);

	if (it != UploadableBufferToUploadFlags.end())
	{
		it->second = true;
	}
}

void UploadableBufferManager::RemoveUploadableBuffer(AManagedByContainer* RemovedBuffer)
{
	AUploadableBuffer* RemovedUploadBuffer = (AUploadableBuffer*)RemovedBuffer;

	for (auto it = UploadableBufferToUploadFlags.begin(); it != UploadableBufferToUploadFlags.end(); ++it)
	{
		if (it->first.get() == RemovedUploadBuffer)
		{
			UploadableBufferToUploadFlags.erase(it);
			break;
		}
	}
}

void UploadableBufferManager::Update(const float& DeltaTimeIn)
{
	for (auto& UploadableBufferToUploadFlag : UploadableBufferToUploadFlags)
	{
		if (UploadableBufferToUploadFlag.second)
		{
			const shared_ptr<AUploadableBuffer> UploadableBuffer = UploadableBufferToUploadFlag.first;
			UploadableBufferToUploadFlags[UploadableBuffer] = false;
			UploadableBuffer->Upload();
		}
	}
}
