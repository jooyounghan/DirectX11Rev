#include "BoundingOBBInformationDrawer.h"
#include "OBBObject.h"

#include <numeric>

using namespace std;
using namespace ImGui;

void BoundingOBBInformationDrawer::DrawInformation()
{
	SeparatorText("Oriented Bounding Box");
	DragFloat3("Half Extends", (float*)&ObjectCached->DescaledExtents, 1.f, 1E-3f, numeric_limits<float>::max());
}
