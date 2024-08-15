#include "AObject.h"
#include <format>

using namespace std;

AObject::AObject()
	: ObjectID(string{ format("{}", (uint64_t)this) })
{
}
