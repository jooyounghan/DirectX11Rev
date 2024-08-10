#include "ViewableInformationDrawer.h"
#include "Viewable.h"

using namespace std;
using namespace ImGui;

void ViewableInformationDrawer::DrawInformation()
{
    float* PointerNearZ = ObjectCached->GetPointerNearZ();
    float& NearZ = *PointerNearZ;
    float* PointerFarZ = ObjectCached->GetPointerFarZ();
    float& FarZ = *PointerFarZ;

    DragFloat("Fov Angle", ObjectCached->GetPointerFovAngle(), 1.f, 30.f, 180.f - 1E-3f);
    DragFloat("NearZ", PointerNearZ, 1.f, static_cast<float>(1E-3f), FarZ - 1E-3f);
    DragFloat("FarZ", PointerFarZ, 1.f, NearZ + 1E-3f, numeric_limits<float>::max());
}
