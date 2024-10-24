#pragma once
#include "AModal.h"
#include "HeaderHelper.h"

class AssetManager;

constexpr size_t AssetFileNameBufferSize = 254;

class CreateAssetFileModal : public AModal
{
public:
	CreateAssetFileModal(const std::string& ModalHeaderNameIn, AssetManager* AssetManagerIn);
	virtual ~CreateAssetFileModal();

protected:
	AssetManager* AssetManagerCached = nullptr;

private:
	std::vector<std::string> AssetItemIdentifiers;
	char AssetFileNameBuffer[AssetFileNameBufferSize];
    const char* InvalidChars = "\\/:*?\"<>|";

protected:
	bool ModalFlag = false;
	MakeSetterGetter(ModalFlag);

protected:
	virtual bool ModalCondition() override;
	virtual void RenderModal() override;

private:
    bool IsValidFileNameChar(char c);
    bool ValidateFileName(const char* fileName);
};

