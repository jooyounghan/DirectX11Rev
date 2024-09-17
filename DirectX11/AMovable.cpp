#include "AMovable.h"
#include "DefineUtility.h"

#include <format>
using namespace std;

AMovable::AMovable()
	:
	RelativePositionID(string{ format("{}", (uint64_t)&RelativePosition) }),
	RelativeAngleID(string{ format("{}", (uint64_t)&RelativeAngle) }),
	RelativeScaleID(string{ format("{}", (uint64_t)&RelativeScale) })
{	
	AutoZeroMemory(RelativePosition);
	AutoZeroMemory(RelativeScale);
	AutoZeroMemory(RelativeAngle);
	RelativeScale.x = 1.f;
	RelativeScale.y = 1.f;
	RelativeScale.z = 1.f;
}
