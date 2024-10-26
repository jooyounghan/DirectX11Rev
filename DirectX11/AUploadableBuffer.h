#pragma once
#include "Delegation.h"

class AUploadableBuffer;

typedef std::function<void(AUploadableBuffer*)> UpdateReadyHandler;

class AUploadableBuffer
{
public:
	virtual void Upload() = 0;

public:
	Delegation<AUploadableBuffer*> UpdateReadyEvent;
};