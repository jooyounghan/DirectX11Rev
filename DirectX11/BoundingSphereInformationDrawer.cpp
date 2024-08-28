#include "BoundingSphereInformationDrawer.h"
#include "BoundingSphereObject.h"

#include <numeric>

using namespace std;
using namespace ImGui;

void BoundingSphereInformationDrawer::DrawInformation()
{
	SeparatorText("Boundging Sphere");
	DragFloat("Radius", ObjectCached->GetPointerDescaledRadius(), 1.f, 1E-3f, numeric_limits<float>::max());
}
