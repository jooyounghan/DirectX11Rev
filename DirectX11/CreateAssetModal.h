#pragma once
#include "AModal.h"
#include "HeaderHelper.h"

class AssetManager;

constexpr size_t AssetNameBufferSize = 254;

class CreateAssetModal : public AModal
{
public:
	CreateAssetModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn);
	virtual ~CreateAssetModal();

protected:
	AssetManager* AssetManagerCached = nullptr;

protected:
	char AssetNameBuffer[AssetNameBufferSize];
	bool IsNotValidFileName = false;
		
protected:
	static const char* InvalidChars;

protected:
	bool ModalFlag = false;
	MakeSetterGetter(ModalFlag);

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;

protected:
	static bool IsValidFileNameChar(char c);
	static bool ValidateFileName(const char* fileName);
};

