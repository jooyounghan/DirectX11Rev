#include "CreateAssetFileModal.h"
#include "AssetManager.h"
#include "MaterialAsset.h"

using namespace std;
using namespace ImGui;

CreateAssetFileModal::CreateAssetFileModal(
	const std::string& ModalHeaderNameIn, 
	AssetManager* AssetManagerIn
)
	: AModal(ModalHeaderNameIn), AssetManagerCached(AssetManagerIn)
{
    AssetItemIdentifiers = {
        MaterialAsset::MaterialAssetKind
    };

    memset(AssetFileNameBuffer, '\0', AssetFileNameBufferSize);
}

CreateAssetFileModal::~CreateAssetFileModal()
{
}

bool CreateAssetFileModal::ModalCondition()
{
	return ModalFlag;
}

void CreateAssetFileModal::RenderModal()
{
	ModalFlag = false;
    static string SelectedAssetKind;

    if (ImGui::BeginCombo("Select Asset Type", SelectedAssetKind.empty() ? "Select Asset Type" : SelectedAssetKind.c_str()))
    {
        for (const string& AssetIdentifier : AssetItemIdentifiers)
        {
            if (ImGui::Selectable(AssetIdentifier.c_str()))
            {
                SelectedAssetKind = AssetIdentifier;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::InputText("Write Asset File Nmae", AssetFileNameBuffer, AssetFileNameBufferSize, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsNoBlank);
    const bool IsNotValidFileName = !ValidateFileName(AssetFileNameBuffer);

    if (IsNotValidFileName)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid File Name");
    }
    else if (SelectedAssetKind.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Select Asset Kind");
    }
    else;

    Separator();
    
    ImGui::BeginDisabled(IsNotValidFileName || SelectedAssetKind.empty());
    if (Button("OK", ImVec2(120, 0)))
    {
        if (SelectedAssetKind == MaterialAsset::MaterialAssetKind)
        {
            shared_ptr<MaterialAsset> MaterialAssetAdded = make_shared<MaterialAsset>(AssetFileNameBuffer, false);
            AssetManagerCached->SerailizeAndAddToContainer(MaterialAssetAdded);
        }
        CloseCurrentPopup();
    }
    SetItemDefaultFocus();
    ImGui::EndDisabled();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0)))
    {
        CloseCurrentPopup();
    }
}

bool CreateAssetFileModal::IsValidFileNameChar(char c)
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

bool CreateAssetFileModal::ValidateFileName(const char* fileName)
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
