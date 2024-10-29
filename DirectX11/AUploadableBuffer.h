#pragma once
#include "Buffer.h"
#include "Delegation.h"
#include "AManagedByConatainer.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

class AUploadableBuffer;

typedef std::function<void(AUploadableBuffer*)> UpdateReadyHandler;

class AUploadableBuffer : public BaseBuffer, public AManagedByContainer
{
public:
	AUploadableBuffer() : DeviceContextCached(App::GGraphicPipeline->GetDeviceContext()) {}

public:
	virtual void Upload() = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> StagingBuffer;
	ID3D11DeviceContext* DeviceContextCached;

public:
	Delegation<AUploadableBuffer*> UpdateReadyEvent;
};