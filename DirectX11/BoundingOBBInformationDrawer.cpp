#include "BoundingOBBInformationDrawer.h"
#include "OrientedBoundingBox.h"

#include <numeric>

using namespace std;
using namespace ImGui;

void BoundingOBBInformationDrawer::DrawInformation()
{
	ImGui::SeparatorText("Oriented Bounding Box");
	DragFloat3("Half Extends", ObjectCached->GetPointerHalfExtends(), 1.f, 1E-3f, numeric_limits<float>::max());
}
