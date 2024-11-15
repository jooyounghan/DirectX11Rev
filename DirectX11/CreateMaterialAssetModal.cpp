#include "CreateMaterialAssetModal.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "MaterialAsset.h"

using namespace std;
using namespace ImGui;

CreateMaterialAssetModal::CreateMaterialAssetModal(const std::string& ModalHeaderNameIn)
	: CreateAssetModal(ModalHeaderNameIn)
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
        App::GAssetManager->SerailizeAndAddToContainer(MaterialAssetAdded);
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
