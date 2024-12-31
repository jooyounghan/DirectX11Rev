#include "ZIndexComparer.h"
#include "ZIndexable.h"

bool ZIndexComparer::operator()(ZIndexable* zIndex1, ZIndexable* zIndex2) const
{
    if (zIndex1->GetZIndex() != zIndex2->GetZIndex())
    {
        return zIndex1->GetZIndex() > zIndex2->GetZIndex();
    }
    return zIndex1 < zIndex2;
}