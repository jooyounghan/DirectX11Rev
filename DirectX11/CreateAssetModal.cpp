#include "CreateAssetModal.h"

using namespace std;
using namespace ImGui;

const char* CreateAssetModal::InvalidChars = "\\/:*?\"<>|";

CreateAssetModal::CreateAssetModal(
    const std::string& ModalHeaderNameIn, 
    AssetManager* AssetManagerIn
)
    : AModal(ModalHeaderNameIn), AssetManagerCached(AssetManagerIn)
{
    memset(AssetNameBuffer, '\0', AssetNameBufferSize);
}

CreateAssetModal::~CreateAssetModal()
{
}

bool CreateAssetModal::ModalCondition()
{
	return ModalFlag;
}

void CreateAssetModal::RenderModal()
{
    ModalFlag = false;

    ImGui::InputText("Write Asset File Name", AssetNameBuffer, AssetNameBufferSize, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsNoBlank);
    IsNotValidFileName = !ValidateFileName(AssetNameBuffer);

    if (IsNotValidFileName)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid File Name");
    }
    else;

    Separator();
}

bool CreateAssetModal::IsValidFileNameChar(char c)
{
    size_t test = strlen(InvalidChars);
    for (size_t idx = 0; idx < strlen(InvalidChars); ++idx)
    {
        if (c == InvalidChars[idx])
        {
            return false;
        }
    }
    return true;
}

bool CreateAssetModal::ValidateFileName(const char* fileName)
{
    if (strlen(fileName) == 0)
    {
        return false;
    }

    size_t test = strlen(fileName);
    for (size_t idx = 0; idx < strlen(fileName); ++idx)
    {
        if (!IsValidFileNameChar(fileName[idx]))
        {
            return false;
        }
    }
    return true;
}
