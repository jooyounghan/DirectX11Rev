#pragma once
#include "Buffer.h"
#include "AUploadableBuffer.h"

#include "HeaderHelper.h"
#include "DefineUtility.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"

template<typename T>
class StructuredBuffer : public Buffer, public AUploadableBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

protected:
	std::vector<T> StagedStructuredData;
	MakeGetter(StagedStructuredData);

public:
	void SetStagedData(const size_t& Index, const T& StagedDataIn);

public:
	virtual void Upload() override;
};

template<typename T>
inline StructuredBuffer<T>::StructuredBuffer()
{
}

template<typename T>
inline StructuredBuffer<T>::~StructuredBuffer()
{
}

template<typename T>
inline void StructuredBuffer<T>::SetStagedData(const size_t& Index, const T& StagedDataIn)
{
	if (StagedStructuredData.size() > Index)
	{
		StagedStructuredData[Index] = StagedDataIn;
		UpdateReadyEvent.Invoke(this);
	}
}

template<typename T>
inline void StructuredBuffer<T>::Upload()
{
}
