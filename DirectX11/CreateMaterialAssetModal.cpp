#include "CreateMaterialAssetModal.h"
#include "AssetManager.h"
#include "MaterialAsset.h"

using namespace std;
using namespace ImGui;

CreateMaterialAssetModal::CreateMaterialAssetModal(
	const std::string& ModalHeaderNameIn, 
	AssetManager* AssetManagerIn
)
	: CreateAssetModal(ModalHeaderNameIn, AssetManagerIn)
{
}

CreateMaterialAssetModal::~CreateMaterialAssetModal()
{
}

void CreateMaterialAssetModal::RenderModal()
{
    CreateAssetModal::RenderModal();

    ImGui::BeginDisabled(IsNotValidFileName);
    if (Button("OK", ImVec2(120, 0)))
    {
        shared_ptr<MaterialAsset> MaterialAssetAdded = make_shared<MaterialAsset>(AssetNameBuffer, false);
        AssetManagerCached->SerailizeAndAddToContainer(MaterialAssetAdded);
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
