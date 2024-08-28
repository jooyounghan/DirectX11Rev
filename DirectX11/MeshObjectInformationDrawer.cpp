#include "MeshObjectInformationDrawer.h"
#include "MeshObject.h"

#include "AssetManager.h"
#include "AMeshAsset.h"

using namespace std;
using namespace ImGui;

MeshObjectInformationDrawer::MeshObjectInformationDrawer(MeshObject* ObjectIn, AssetManager* AssetManagerIn)
    : AInformationDrawer(ObjectIn), AssetManagerCached(AssetManagerIn)
{

}

void MeshObjectInformationDrawer::DrawInformation()
{
	SeparatorText("Mesh Object");


    //if (ImGui::BeginCombo(EntityName, PreviewText, ImGuiComboFlags_WidthFitPreview))
    //{
    //    for (size_t idx = 0; idx < (size_t)NumTransformationSelect; ++idx)
    //    {
    //        const ETransfomationSelect CurrentSelectIdx = static_cast<ETransfomationSelect>(idx);
    //        const bool IsSelected = (SelectedIndexOut == CurrentSelectIdx);
    //        if (ImGui::Selectable(TransformationSelect[idx], IsSelected, NULL))
    //            SelectedIndexOut = CurrentSelectIdx;

    //        if (IsSelected)
    //            ImGui::SetItemDefaultFocus();
    //    }
    //    ImGui::EndCombo();
    //}
}
