#include "CreateAssetFileModal.h"
#include "AAssetFile.h"

using namespace std;
using namespace ImGui;

CreateAssetFileModal::CreateAssetFileModal(
	const std::string& ModalHeaderNameIn, 
	AssetManager* AssetManagerIn
)
	: AModal(ModalHeaderNameIn), AssetManagerCached(AssetManagerIn)
{
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
    Separator();
    if (Button("OK", ImVec2(120, 0)))
    {
        EAssetType






        CloseCurrentPopup();
    }
    SetItemDefaultFocus();
    SameLine();
    if (Button("Cancel", ImVec2(120, 0)))
    {
        CloseCurrentPopup();
    }
}
