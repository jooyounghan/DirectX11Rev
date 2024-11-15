#include "MaterialInformationDrawer.h"
#include "AMeshObject.h"

#include "GlobalVariable.h"
#include "AssetManager.h"
#include "MaterialAsset.h"
#include "AssetSelectHelper.h"

#include "UIVariable.h"

using namespace std;
using namespace ImGui;

MaterialInformationDrawer::MaterialInformationDrawer(AMeshObject* ObjectIn)
    : AInformationDrawer(ObjectIn)
{
}

void MaterialInformationDrawer::DrawInformation()
{
	SeparatorText("Materials");

    const unordered_map<string, shared_ptr<MaterialAsset>>& ManagingMaterials = App::GAssetManager->GetManagingMaterials();
    const vector<shared_ptr<MaterialAsset>>& MaterialInstances = ObjectCached->GetMaterialAssetInstances();

    for (size_t idx = 0; idx < MaterialInstances.size(); ++idx)
    {
        MaterialAsset* MaterialInstance = ObjectCached->GetMaterialAssetInstance(idx);

        string MaterialSlotID;
        if (MaterialInstance != nullptr)
        {
            MaterialSlotID = MaterialInstance->GetAssetName() + to_string(idx);
        }
        else
        {
            MaterialSlotID = "Material" + to_string(idx);
        }

        PushID(MaterialSlotID.c_str());

        Image(nullptr, UISize::FileSize);

        SameLine();

        shared_ptr<MaterialAsset> Result = AssetSelectHelper::SelectAsset(ManagingMaterials, MaterialInstance, MaterialSlotID, "Material Asset", "Select Material Asset");
        if (Result != nullptr) ObjectCached->SetMaterialAsset(idx, Result);
        PopID();
    }
}
