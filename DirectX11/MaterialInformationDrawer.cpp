#include "MaterialInformationDrawer.h"
#include "MaterialAsset.h"
#include "AssetManager.h"
#include "AMeshObject.h"

#include "UIVariable.h"

using namespace std;
using namespace ImGui;

MaterialInformationDrawer::MaterialInformationDrawer(AMeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{
}

void MaterialInformationDrawer::DrawInformation()
{
	SeparatorText("Materials");

    const unordered_map<string, shared_ptr<MaterialAsset>>& ManagingMaterials = AssetManagerCached->GetManagingMaterials();
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

        shared_ptr<MaterialAsset> Result = SelectAsset(ManagingMaterials, MaterialInstance, "Material Asset", "Choose Material Asset");
        if (Result != nullptr) ObjectCached->SetMaterialAsset(idx, Result);
        PopID();
    }
}
