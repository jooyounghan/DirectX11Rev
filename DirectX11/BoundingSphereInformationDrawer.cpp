#include "BoundingSphereInformationDrawer.h"
#include "BoundingSphere.h"

#include <numeric>

using namespace std;
using namespace ImGui;

void BoundingSphereInformationDrawer::DrawInformation()
{
	ImGui::SeparatorText("Boundging Sphere");
	DragFloat("Radius", ObjectCached->GetPointerRadius(), 1.f, 1E-3f, numeric_limits<float>::max());
}
